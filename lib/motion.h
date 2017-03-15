#ifndef MOTION
  #define MOTION

  #include <Arduino.h>
  #include <Servo.h>
  #include "sensors.h"

//MOTOR CONTROL
#define MDR0 7
#define MDR1 8
#define MDL0 12
#define MDL1 11
#define PWMR 5
#define PWML 9

#define WALLF_LIMIT_PIN 18
#define SERVOPIN 23
#define SERVOHOME 90
#define SERVO_DISPLACE 15
#define SERVO_DELAY 200

//PID
  class PID
  {

    public:
      float pidd[4]; //err, last error, running integral, adjust
      float w[3];

      void set_pid(float p, float i, float d)
      {
        w[0] = p;w[1] = i;w[2] = d;
        pidd[0] = 0; pidd[1] = 0; pidd[2] = 0;
      }

      float slice(float err, int dtl)
      {
        if(dtl != 0)
        {
          pidd[1] = pidd[0];
          pidd[0] = err;
          pidd[2] += ((pidd[0] + pidd[1])/2)*dtl;

          pidd[3] = pidd[0]*w[0] + pidd[2]*w[1] + (pidd[0] - pidd[1])/dtl * w[2];
        }

        return pidd[3];
      }
  };

class motion
{
  private:
    timeu t;
    PID pida, pidlf, pidlfset;
    line mline;
    Servo deps;

    void mr_out(int pwr)
    {
      pwr = constrain(pwr, -255, 255);
      if(pwr > 0){digitalWrite(MDR0, LOW);  digitalWrite(MDR1, HIGH);}
      else{digitalWrite(MDR0, HIGH);  digitalWrite(MDR1, LOW);}
      analogWrite(PWMR, abs(pwr));
    }

    void ml_out(int pwr)
    {
      pwr = constrain(pwr, -255, 255);
      if(pwr > 0){digitalWrite(MDL0, LOW);  digitalWrite(MDL1, HIGH);}
      else{digitalWrite(MDL0, HIGH);  digitalWrite(MDL1, LOW);}
      analogWrite(PWML, abs(pwr));
    }

  public:
    motion(line a_mline)
    {
      pida.set_pid(10, 0, 300);
      pidlf.set_pid(.5, 0, 20);
      pidlfset.set_pid(.6, 200);
      mline = a_mline;

      deps.attach(SERVOPIN);
      deps.write(90);
    }

    void depr()
    {
      deps.write(SERVOHOME - SERVO_DISPLACE);
      delay(SERVO_DELAY);
      deps.write(SERVOHOME);
    }

    void depl()
    {
      deps.write(SERVOHOME + SERVO_DISPLACE);
      delay(SERVO_DELAY);
      deps.write(SERVOHOME);
    }

    void arc(float r, int s)
    {
      mr_out( s*(1-(WB_L/(2*r))) );
      ml_out( s*(1+(WB_L/(2*r))) );
    }

    void wf_limit(int s, int mode)
    {
      int logic = (digitalRead(WALLF_LIMIT_PIN) == HIGH);
      logic = (logic == 1) - (logic == 0);

      int adj_l = 0.2*s
      mr_out(gs - logic * adj_l);
      ml_out(gs + logic * adj_l);
    }

    void drive_straight(int s)
    {
      adj = pida.slice(motion[0], dt);
      mr_out(s - adj);
      ml_out(s + adj);
    }

    void line_follow(int s)
    {
      pos = mline.get_pos();
      if(density != 0){
        digitalWrite(13, LOW);
        adj = pidlf.slice(CENTEROFLINE - pos, dt);
        Serial.println(gs + adj);
        mr_out(gs + adj);
        ml_out(gs - adj);
      }else{
        digitalWrite(13, HIGH);
        mr_out(0);
        ml_out(0);
        delay(300);
        digitalWrite(13, LOW);
        delay(100);
    }

    void line_follow_settle(int s)
    {

    }
};


void calibrate()
{
  int tval;
  while(eval_dip(0xFF, 0x0F))
  {
    for(int i = 0; i < NUMLSENSORS; i++)
    {
      tval = analogRead(svPins[i]);

      if(tval < low[i]) {low[i] = tval;}
      else if(tval > high[i]) {high[i] = tval;}
    }

    #ifdef DEBUG
      debug_calibration();
    #endif
  }

  for(int i = 0; i < NUMLSENSORS; i++)
  {
    sv_scale[i] = POSSCALE/(high[i] - low[i]);
  }

  #ifdef DEBUG
    while(1){debug_line_sensor(1);}
  #endif
}

#endif
