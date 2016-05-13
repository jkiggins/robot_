#include "asee_lib.h"

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

#include <Arduino.h>
#include <limits.h>
#include <Servo.h>
#include <ZX_Sensor.h>

//Calibration VARS
float sv_scale[8];
long low[8];
long high[8];

long CENTEROFLINE; //center of line value given calibration high and low values

float adj; //global pid adjust variable

//Motion and position VARS
float motion[3] = {0,0,0}; //angle, right motor ticks/second, left motor ticks/second

float dd; //distance travled, used for relative distance manuvers
int dd_flag; //when set to 1 dd is incremented with encoders

Servo deps; //deposite servo variable
elapsedMillis em;

//objects to handle quadrature encoders on dual motors
Encoder mldecode(30,29);
Encoder mrdecode(26,27);

int gs = 0; //global speed to be shared by all async methods
int async_state = -1;

//PID ################################
  void PID::set_pid(float p, float i, float d)
  {
    w[0] = p;w[1] = i;w[2] = d;
    pidd[0] = 0; pidd[1] = 0; pidd[2] = 0;
  }

  float PID::slice(float err, float s, float dtl)
  {
    if(dtl != 0)
    {
      pidd[1] = pidd[0];
      pidd[0] = err;
      pidd[2] += ((pidd[0] + pidd[1])/2)*dtl;

      pidd[2] = constrain(pidd[2], -s/w[1], s/w[1]);

      pidd[3] = pidd[0]*w[0] + pidd[2]*w[1] + (pidd[0] - pidd[1])/dtl * w[2];
    }

    return pidd[3];
  }

//LF ################################
  //VARS
    PID pidlf;
    long svals[8];
    long w, wsum, pos;
    const int svPins[NUMLSENSORS] = {A8,A7,A6,A3,A2,A1,A0,A17};

  void read_sv()
  {
    for(int i = 0; i < NUMLSENSORS; i++)
    {
      svals[i] = (analogRead(svPins[i]) - low[i])*sv_scale[i];
    }
  }

  int read_line()
  {
    read_sv();
    w = 0;wsum = 0;

    for(long i = 0; i < NUMLSENSORS; i++)
    {
      w += svals[i];
      wsum += svals[i]*i*POSSCALE;
    }

    return (wsum/w);
  }

  int lost_line()
  {
    return (w <= 5);           
  }

  void lf(int s)
  {
    async_state = LINEF;
    gs = s;
  }

  void stop_sensor(int sn, int mode)
  {
    int snval = analogRead(svPins[sn]);
    int osnval = snval;
    async_reset();

    while(1)
    {
      if(mode == 0)
      {
        if((osnval < HSCALE*high[sn]) && (snval >= HSCALE*high[sn])) break;
      }
      else if(mode == 1)
      {
        if((osnval > low[sn]) && (snval <= low[sn])) break;
      }

      osnval = snval;
      snval = analogRead(svPins[sn]);

      async();
    }
  }

//DR ################################
  //vars
    PID pida;
    float err;

  void dr(float s)
  {
    async_state = DRIVED;
    gs = s;
  }

  void stop_dd(float dist)
  {
    dd = 0;
    dd_flag = 1;
    async_reset();

    int dir = sign_f(dist);

    while(dd*dir < abs(dist)){async();}
  }

//ARC
  void rotate(int s, int mode) //0 - both motors, 1 - right motor on, 2 - left motor on
  {
    async_state = -1;
    if(mode == 0)
    {
      mr_out(s);
      ml_out(-s);
    }
    else if(mode == 1)
    {
      mr_out(s);
      ml_out(0);
    }
    else
    {
      ml_out(-s);
      mr_out(0);
    }
  }

  void arc(float r, int s)
  {
    async_state = -1;
    mr_out( s*(1-(WB_L/(2*r))) );
    ml_out( s*(1+(WB_L/(2*r))) );
  }

  void stop_deg(float a)
  {
    motion[0] = 0;
    int dir = sign_f(a);

    while(motion[0]*dir < abs(a))
    {
      async();
    }
  }

//XZDISTANCE
  const int ZX_ADDR = 0x10;  // ZX Sensor I2C address
  ZX_Sensor zxs = ZX_Sensor(ZX_ADDR);
  uint8_t z_pos;

  int get_dist()
  {
    if(zxs.positionAvailable())
    {
      z_pos = zxs.readZ();
      if(z_pos != ZX_ERROR)
      {
        return z_pos;
      }
    }

    return 1000;
  }

  void stop_zx(int dist, int mode) //0 - stop when under 1 - stop when over
  {
    async_reset();
    int rtdist = get_dist();

    while(1)
    {
      if(rtdist != 0)
      {
        if(mode == 0 && rtdist <= dist){break;}
        else if(mode == 1 && rtdist >= dist){break;}
      }

      async();
      rtdist = get_dist();
    }
  }

//UPDATE
  //VARS
    long rtickL, ltickL;

  void update(float dt)
  {
    rtickL = mrdecode.read();
    mrdecode.write(0);

    ltickL = mldecode.read();
    mldecode.write(0);

    if(dd_flag == 1)
    {
      dd += ((ltickL + rtickL)/2)*MPT;
    }

    if(rtickL != ltickL)
    {
      motion[0] += eval_angle();
    }
  }

  float eval_angle()
  {
    float r = (-WB_L/2)*((rtickL + ltickL)/(rtickL - ltickL));
    float da = 0;

    if(r < 0)
    {
      da = -(rtickL * MPT)/(r-WB_L/2);
    }
    else
    {
      da = -(ltickL * MPT)/(r+WB_L/2);
    }

    return da;
  }

//INIT
  void init_a()
  {
    pinMode(MDR0, OUTPUT);pinMode(MDR1, OUTPUT);pinMode(MDL0, OUTPUT);pinMode(MDL1, OUTPUT);
    pinMode(PWMR, OUTPUT);
    pinMode(PWML, OUTPUT);
    pinMode(13, OUTPUT);

    digitalWrite(13, HIGH);

    deps.attach(SERVOPIN);
    deps.write(SERVOHOME);
    dd_flag = 1;
    Serial.begin(9600);

    //zxs.init();
  }

//ASYNC
  float dt;

  void async_reset()
  {
    switch(async_state)
    {
      case LINEF:
        pidlf.set_pid(.7, 0, 18);
        w = 0; wsum = 0; adj = 0; pos = CENTEROFLINE;
        break;
      case DRIVED:
        pida.set_pid(500, 0, 50);
        dd = 0;motion[0] = 0; adj = 0;dd_flag = 1;
        break;
    }
    em = 0;
  }

  void async()
  {
    dt = em;
    em = 0;

    update(dt);

    switch(async_state)
    {
      case LINEF:
        pos = read_line();

        if(!lost_line())
        {
          adj = pidlf.slice(CENTEROFLINE - pos, gs, dt);
          adj = constrain(adj, -gs, gs);
        }

        mr_out(gs + adj);
        ml_out(gs - adj);
        break;
      /////////////////////////////////////////////////////////////////////
      case DRIVED:
        adj = pida.slice(motion[0], 30, dt);
        mr_out(gs - adj);
        ml_out(gs + adj);
        break;
    }
  }

  void break_mots()
  {
    mr_out(0);
    ml_out(0);
  }

//TOOLBOX
  void mr_out(int pwr)
  {
    pwr = constrain(pwr, -255, 255);

    if(pwr > 0)
    {
      digitalWrite(MDR0, LOW);
      digitalWrite(MDR1, HIGH);
    }
    else if(pwr == 0)
    {
      digitalWrite(MDR0, HIGH);
      digitalWrite(MDR1, HIGH);
    }
    else
    {
      digitalWrite(MDR0, HIGH);
      digitalWrite(MDR1, LOW);
    }

    analogWrite(PWMR, abs(pwr));
  }

  void ml_out(int pwr)
  {
    pwr = constrain(pwr, -255, 255);

    if(pwr > 0)
    {
      digitalWrite(MDL0, LOW);
      digitalWrite(MDL1, HIGH);
    }
    else if(pwr == 0)
    {
      digitalWrite(MDL0, HIGH);
      digitalWrite(MDL1, HIGH);
    }
    else
    {
      digitalWrite(MDL0, HIGH);
      digitalWrite(MDL1, LOW);
    }

    analogWrite(PWML, abs(pwr));
  }

  void depr()
  {
    deps.write(SERVOHOME + 15);
    delay(800);
    deps.write(SERVOHOME);
  }

  void depl()
  {
    deps.write(SERVOHOME - 15);
    delay(800);
    deps.write(SERVOHOME);
  }

  void calibrate(int s)
  {
    gs = s;
    async_state = DRIVED;
    async_reset();
    
    int tval;

    while(dd < 1)
    {
      for(int i = 0; i < NUMLSENSORS; i++)
      {
        tval = analogRead(svPins[i]);

        if(tval < low[i]) {low[i] = tval;}
        else if(tval > high[i]) {high[i] = tval;}
      }
      async();
    }

    break_mots();

    long w_l = 0;
    long wsum_l = 0;

    for(int i = 0; i < NUMLSENSORS; i++)
    {
      if(i < 3 && i > 4)
      {
        w_l += low[i];
        wsum_l += low[i]*i*POSSCALE;
      }
      else
      {
        w_l += high[i];
        wsum_l += high[i]*i*POSSCALE;
      }

      sv_scale[i] = POSSCALE/(high[i] - low[i]);
    }

    CENTEROFLINE = wsum_l/w_l;

    Serial.println(CENTEROFLINE);
  }

  int sign_f(float val)
  {
    if(val < 0){return -1;}
    else if(val > 0){return 1;}
    else{return 0;}
  }

  int sign_i(int val)
  {
    if(val < 0){return -1;}
    else if(val > 0){return 1;}
    else{return 0;}
  }

//EOF