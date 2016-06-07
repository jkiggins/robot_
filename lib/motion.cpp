#include <Arduino.h>
#include <Servo.h>

#include "asee_lib.h"
#include "sensors.h"

Servo deps; //deposite servo variable

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
    char sv_char;


  void lf(int s)
  {
    async_state = LINEF;
    gs = s;
  }

  void lf_settle(int s)
  {
    async_state = LFSET;
    gs = s;
  }

  void set_last_line(int ll)
  {
    last_line = ll;
  }

  void lf_async(float dt)
  {
  	pos = read_line();
    if(get_density() != 0)
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
  }

//DR
  PID pida;
  float err;

  void dr(int s)
  {
    async_state = DRIVED;
    gs = s;
  }

  void dr_async()
  {
  	adj = pida.slice(motion[0], 30, dt);
    mr_out(gs - adj);
    ml_out(gs + adj);
  }

//WF
  void wf(int speed, int dist, int mode) //mode = 0 wall is on left, mode = 1 wall is on right
  {
    gs = speed;
    gd = dist;
    gm = mode;
    async_state = WALLF;
  }

  void wf_async(float dt)
  {
  	int d_l = get_dist_w();

    gc += dt;
    gc %= OTIME;

    if(d_l < 80)
    {
      adj = pidwf.slice(gd - d_l, 10, dt);
      mr_out(gs - adj);
      ml_out(gs + adj);
      gc = 0;
    }
    else if(gm == 0)
    {
      if(gc < OTIME/3.5){mr_out(SLOW);ml_out(-SLOW);}
      else{ml_out(SLOW);mr_out(SLOW*RSCALE);}          
    }
    else if(gm == 1)
    {
      if(gc < 500){ml_out(SLOW);mr_out(SLOW*RSCALE);}
      else{mr_out(SLOW);ml_out(SLOW*RSCALE);}  
    }
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

//SPEED CONTROL
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

  void break_mots()
  {
    mr_out(0);
    ml_out(0);
  }

//MISC
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

//INIT
	void init_a()
  {
    pinMode(MDR0, OUTPUT);pinMode(MDR1, OUTPUT);pinMode(MDL0, OUTPUT);pinMode(MDL1, OUTPUT);
    pinMode(PWMR, OUTPUT);
    pinMode(PWML, OUTPUT);
    pinMode(13, OUTPUT);

    deps.attach(SERVOPIN);
    deps.write(SERVOHOME);
    Serial.begin(9600);
  }
