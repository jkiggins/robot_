#include "asee_lib.h"

#include <Arduino.h>
#include <limits.h>
#include <Servo.h>
#include <elapsedMillis.h>



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
        pidlf.set_pid(.5, 0, 170);
        w = 0; wsum = 0; adj = 0; pos = CENTEROFLINE;
        break;
      case LFSET:
        pidlf.set_pid(.5,0,200);
        async_state = LINEF;
    }
    get_dt();
  }

  void async()
  {
    dt = get_dt();

    switch(async_state)
    {
      case LINEF:
        pos = read_line();
        if(density != 0)
        {
          adj = pidlf.slice(CENTEROFLINE - pos, dt);
          mr_out(gs + adj);
          ml_out(gs - adj);
        }
        else
        {
          mr_out(-last_line*TURN_SPEED);
          ml_out(last_line*TURN_SPEED);
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