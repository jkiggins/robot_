#include "asee_lib.h"
/*
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
*/
#include <Arduino.h>
#include <limits.h>
#include <Servo.h>

#include "sensors.h"
#include "motion.h"
#include "control.h"
#include "toolbox.h"

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
    //dd_flag = 1;
    Serial.begin(9600);

    last_line = 1;
  }

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
      /*case DRIVED:
        pida.set_pid(500, 0, 50);
        dd = 0;motion[0] = 0; adj = 0;dd_flag = 1;
        break;*/
      case WALLF:
        pidwf.set_pid(.4, 0, 12);
        break;
    }
    em = 0;
  }

  void async()
  {

    update(dt);

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
/*////////////////////////////////////////////////////////////////////
      case DRIVED:
        adj = pida.slice(motion[0], 30, dt);
        mr_out(gs - adj);
        ml_out(gs + adj);
        break;
/*///////////////////////////////////////////////////////////////////
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

//EOF