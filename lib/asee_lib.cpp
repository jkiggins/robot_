#include "asee_lib.h"

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

#include <Arduino.h>
#include <limits.h>
#include <Servo.h>

//Calibration VARS
float sv_scale[8];
long low[8] = {1025, 1025, 1025, 1025, 1025, 1025,1025, 1025};
long high[8] = {0, 0, 0, 0, 0, 0, 0, 0};

//calibrate values for distance sensor
int dlow = 1025;
int dhigh = 0;
float dscale;

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
int gd = 0; //global distance away from the wall to use when wall following
int gm = 0; //global mode variable to be used by any method
int gadj = 0;
int async_state = -1;

#include "motion.h"

//XZDISTANCE

  //const int ZX_ADDR = 0x10;  // ZX Sensor I2C address
  //ZX_Sensor zxs = ZX_Sensor(ZX_ADDR);
  //uint8_t z_pos;

  PID pidwf;

  int get_dist()
  {
    return (dhigh - analogRead(AD_PIN))*dscale;
  }

  void wf(int speed, int dist, int mode) //mode = 0 wall is on left, mode = 1 wall is on right
  {
    gs = speed;
    gd = dist;
    gm = mode;
    async_state = WALLF;
  }

  void wf_limit(int speed, int mode)
  {
    gs = speed;
    gm = mode;
    async_state = WALLF_LIMIT;
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

    if(r > 100) return 0;

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
    pinMode(WF_PIN, INPUT);
    pinMode(BOX_LIMIT_PIN, INPUT);

    deps.attach(SERVOPIN);
    deps.write(SERVOHOME);
    dd_flag = 1;
    Serial.begin(9600);

    last_line = 1;

    //zxs.init();
  }

//CONTROL
#include "control.h"

//ASYNC
  float dt;

  void async_reset()
  {
    switch(async_state)
    {
      case LINEF:
        pidlf.set_pid(.9, 0, 100);
        w = 0; wsum = 0; adj = 0; pos = CENTEROFLINE;
        break;
      case LFSET:
        pidlf.set_pid(.5,0,175);
        async_state = LINEF;
      case DRIVED:
        pida.set_pid(500, 0, 50);
        dd = 0;motion[0] = 0; adj = 0;dd_flag = 1;
        break;
      case WALLF:
        pidwf.set_pid(.4, 0, 12);
        break;
    }
    em = 0;
  }

  void async()
  {
    dt = em;
    em = 0;

    update(dt);
    digitalWrite(13, LOW);

    switch(async_state)
    {
      case LINEF:
        pos = read_line();
        if(density != 0)
        {
          adj = pidlf.slice(CENTEROFLINE - pos, gs, dt);
          mr_out(gs + adj);
          ml_out(gs - adj);
        }
        else
        {
          digitalWrite(13, HIGH);
          mr_out(-last_line*160);
          ml_out(last_line*160);
        }

        break;
/////////////////////////////////////////////////////////////////////
      case DRIVED:
        adj = pida.slice(motion[0], 30, dt);
        mr_out(gs - adj);
        ml_out(gs + adj);
        break;
////////////////////////////////////////////////////////////////////
      case WALLF:
      {
          int d_l = get_dist();
          if(d_l < 80)
          {
            adj = pidwf.slice(gd - d_l, 10, dt);
            mr_out(gs + adj);
            ml_out(gs - adj);
            delay(100);
          }
          else if(gm == 0)
          {
            /*
            rotate(SLOW, 1);
            stop_deg(.2);
            dr(SLOW);
            stop_dd(.2);
            rotate(-SLOW, 1);
            stop_deg(-.2);
            */

            async_state = WALLF;
          }
        }
        break;
////////////////////////////////////////////////////////////////////
      case WALLF_LIMIT:
      {
        int logic = (digitalRead(18) == HIGH);
        Serial.println(logic);
        logic = (logic == 1) - (logic == 0);

        delay(100);

        int adj_l = 0.2*gs;
        mr_out(gs - logic * adj_l);
        ml_out(gs + logic * adj_l);
        break;
      }
    }
  }

  void no_state()
  {
    async_state = -1;
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
    deps.write(SERVOHOME - 45);
    delay(250);
    deps.write(SERVOHOME);
  }

  void depl()
  {
    deps.write(SERVOHOME + 45);
    delay(250);
    deps.write(SERVOHOME);
  }

  void calibrate(int s, float d)
  {
    int tval;

    //calibrate line sensor
    gs = s;
    async_state = DRIVED;
    async_reset();
    

    while(dd < d)
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

    for(int i = 0; i < NUMLSENSORS; i++)
    {
      sv_scale[i] = POSSCALE/(high[i] - low[i]);
    }
  }

  void go()
  {
    async_reset();
    while(1){async();}
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

  void blink_led()
  {
    digitalWrite(13, HIGH);
    delay(300);
    digitalWrite(13, LOW);
    delay(300);
  }

//EOF