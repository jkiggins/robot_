#include "asee_lib.h"

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

#include <Arduino.h>
#include <limits.h>
#include <Servo.h>

//VARIABLES
long high = 0;
long low = 0;
long lpos, hpos, llv;
float adj; //pid adjust variable
float motion[3] = {0,0,0}; //angle, mrtps, mltps

int sindex = 0;
float dd;
int dd_flag;

long CENTEROFLINE;

Servo deps;
elapsedMillis em;

Encoder mldecode(30,29);
Encoder mrdecode(26,27);

//CLASSES

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

  void lf_reset()
  {
    pidlf.set_pid(.7, 0, 18);
    w = 0; wsum = 0; adj = 0; pos = CENTEROFLINE;
  }

  void read_sv()
  {
    for(int i = 0; i < NUMLSENSORS; i++)
    {
      svals[i] = analogRead(svPins[i]);
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
    return (w < llv);           
  }

  void lf_slice(int s, float dt)
  {
    pos = read_line();

    if(!lost_line())
    {
      adj = pidlf.slice(CENTEROFLINE - pos, s, dt);
      adj = constrain(adj, -s, s);
    }

    mr_out(s + adj);
    ml_out(s - adj);
  }

//DR ################################
  //vars
    PID pida;
    float err;

  void dr_reset()
  {
    pida.set_pid(500, 0, 50);
    dd = 0;motion[0] = 0; adj = 0;dd_flag = 1;
  }

  int dr_slice(float d, float s, float dt)
  {
    if(dd*sign_f(d) < abs(d))
    {
      adj = pida.slice(motion[0], 30, dt);
      mr_out(s - adj);
      ml_out(s + adj);

      return 0;
    }
    else{dd_flag = 0;return 1;}
  }

//ARC
  void arc_reset()
  {
    motion[0] = 0;
  }

  void rotate(float deg, int speed, int mode) //0 - both motors, 1 - right motor on, 2 - left motor on
  {
    int dir = sign_f(deg);

    while(motion[0] * dir < abs(deg))
    {
      mr_out(speed);
      ml_out(dir*speed);
    }

    break_mots();
  }

  void arc(float r, float s, int speed)
  {
    pr = speed*(1-(WB/(2*r)));
    pl = speed*(1+(WB/(2*r)));

    arc_reset();
    dd = 0;
    dd_flag = 1;

    while(dd < s)

    dd_flag = 0;
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

  void turnr(int mode)
  {

  }

  void turnl(int mode)
  {

  }

  void lf_d(int speed, float d)
  {

  }

  void lf_t(int speed, int mils)
  {

  }

  void dr_zx(int speed, int dist)
  {

  }
  void lf_zx(int speed, int zx)
  {

  }

  void rotate_to_sensor(int speed, int pin, int mode)
  {

  }
  void rotate_to_pos(int speed, int pos)
  {

  }

  void break_mots()
  {

  }

//STATE
  //VARS
    char order[500];
    int I[500];
    float F[500];
    int osindex = -1;
    float dt, xzdist;
    int argi;
    float argf;
    int N = 0;
    int time;

  void states()
  {
    dt = em;
    em = 0;

    update(dt);

    if(sindex >= N)
    {
      return;
    }

    if(sindex != osindex)
    {
      argi = I[sindex];
      argf = F[sindex];

      switch(order[sindex])
      {
        case LFCORNER:
        case LFZX:
          lf_reset();
          break;
        case LFD:
          lf_reset();
          dd = 0;motion[0] = 0;dd_flag = 1;
          break;
        case DRIVED:
          dr_reset();
          break;
        case DRIVET:
          time = 0;
          dr_reset();
          dd_flag = 0;
          break;
      }
    }    
    osindex = sindex;

    switch(order[sindex])
    {
      case 0:
        calibrate();
        sindex++;
        break;
//////////////////////////////////////////////////////////
      case 1: //line follow to corner I{speed} F{}
        lf_slice(argi, dt);
        if(svals[NUMLSENSORS - 1] >= .9*high || svals[0] >= .9*high) sindex++;
        break;
//////////////////////////////////////////////////////////
      case 2: //line follow distance I{speed} F{distance}
        lf_slice(argi, dt);
        
        if(dd > argf){ dd_flag = 0; sindex++;}
        break;
//////////////////////////////////////////////////////////
      case 3: //line follow until lost line I{speed} F{}
        lf_slice(argi, dt);
        if(lost_line()) sindex++;
        break;
//////////////////////////////////////////////////////////
      case 4: //start rotation I{speed} f{}
        motion[0] = 0; dd = 0;
        mr_out(argi);
        ml_out(-argi);
        sindex++;
        break;
//////////////////////////////////////////////////////////
      case STPLIN: //stop rotation on a line I{pin} f{}
        if(analogRead(svPins[argi]) > .7*high) sindex++;
        break;
//////////////////////////////////////////////////////////
      case 6: //stop rotation at degrees I{} f{angle} angle must coorespond to direction of rotation, no training wheels
        if(motion[0]*sign_f(argf) > abs(argf)) sindex++;
        break;
//////////////////////////////////////////////////////////
      case 7: //break motors I{} F{}
        mr_out(0);
        ml_out(0);
        sindex++;
        break;
//////////////////////////////////////////////////////////
      case 8:
        //TODO reverse sonic code here
        break;
//////////////////////////////////////////////////////////
      case 9: //I{} F{}
        deps.write(SERVOHOME + 45);
        delay(1000);
        deps.write(SERVOHOME);
        em = 0;
        sindex++;
        break;
//////////////////////////////////////////////////////////
      case 10: //I{} F{}
        deps.write(SERVOHOME - 45);
        delay(1000);
        deps.write(SERVOHOME);
        em = 0;
        sindex++;
        break;
//////////////////////////////////////////////////////////
      case 11: //drive distance I{speed} F{distance}
        sindex += dr_slice(argf, argi, dt);
        break;
//////////////////////////////////////////////////////////
      case DRIVET: //drive for time I{speed} F{time}
        dr_slice(1, argi, dt);
        time += dt;
        if(time > argf) sindex++;
        break;
//////////////////////////////////////////////////////////
      default:
        break;
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
      update_curve();
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

    motion[0] += da;
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

//TOOLBOX
  void mr_out(int pwr)
  {
    pwr = constrain(pwr, -255, 255);

    if(pwr > 0)
    {
      digitalWrite(MDR0, LOW);
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
    else
    {
      digitalWrite(MDL0, HIGH);
      digitalWrite(MDL1, LOW);
    }

    analogWrite(PWML, abs(pwr));
  }

  void calibrate()
  {
    long holder[8];
    long mid_range, lc, hc;
    hc = 0;lc = 0;

    for(int i = 0; i < NUMLSENSORS; i++)
    {
      holder[i] = (analogRead(svPins[i]) + analogRead(svPins[i]) + analogRead(svPins[i]))/3;
      //holder[i] = analogRead(svPins[i]);
      if(holder[i] < low){low = holder[i];}
      else if(holder[i] > high){high = holder[i];}
    }

    mid_range = low + (high - low)/2;

    low = 0;high = 0;

    for(int i = 0; i < NUMLSENSORS; i++)
    {
      if(holder[i] < mid_range)
      {
        low += holder[i];
        lc++;
      }
      else
      {
        high += holder[i];
        hc++;
      }
    }

    low/=lc;
    high/=hc;

    CENTEROFLINE = (high*(7*POSSCALE) + low*(21*POSSCALE))/(high * 2 + low * 6);

    llv = low*NUMLSENSORS*1.05;
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