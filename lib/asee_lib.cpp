#include "asee_lib.h"
<<<<<<< HEAD
/*
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
*/
=======

>>>>>>> new_motors
#include <Arduino.h>
#include <limits.h>
#include <Servo.h>
#include <elapsedMillis.h>
#include <EEPROMex.h>

#include "digital_edge.h"

#include "sensors.h"
#include "motion.h"
#include "control.h"
#include "toolbox.h"

//INIT
  void init_a()
  {
    pinMode(MDR0, OUTPUT);pinMode(MDR1, OUTPUT);pinMode(MDL0, OUTPUT);pinMode(MDL1, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(WF_PIN, INPUT);
    pinMode(BOX_LIMIT_PIN, INPUT);

    deps.attach(SERVOPIN);
    deps.write(SERVOHOME);

    dd_flag = 1;

    Serial.begin(9600);

    last_line = 1;

    #ifdef INC_PID
      pinMode(PINC_PIN, INPUT_PULLUP);
      pinMode(DINC_PIN, INPUT_PULLUP);
    #endif
  }

//ASYNC
  float dt;

  void async_reset()
  {
    switch(async_state)
    {
      case LINEF:
        pidlf.set_pid(.8, 0, 80);
        w = 0; wsum = 0; adj = 0; pos = CENTEROFLINE;

        #ifdef INC_PID
          //pidlf.set_pid(EEPROM.readFloat(0), 0, EEPROM.readFloat(4));
          pidlf.set_pid(.8, 0, 80);
        #endif

        break;
      case LFSET:
        pidlf.set_pid(.4,0,220);
        async_state = LINEF;
      case DRIVED:
        pida.set_pid(500, 0, 50);
        dd = 0;motion[0] = 0; adj = 0;dd_flag = 1;
        break;
    }
    get_dt();
  }

  void async()
  {
     dt = get_dt();
    update(dt);

    switch(async_state)
    {
      case LINEF:
        #ifdef INC_PID
          pidlf.inc_pid();
        #endif
        pos = read_line();        
        if(density != 0)
        {
          adj = pidlf.slice(CENTEROFLINE - pos, dt);
          mr_out(gs + adj);
          ml_out(gs - adj);
        }
        else
        {
          #ifndef INC_PID
            mr_out(-last_line*TURN_SPEED);
            ml_out(last_line*TURN_SPEED);
          #else
            mr_out(0);
            ml_out(0);
          #endif
        }

        break;
////////////////////////////////////////////////////////////////////
      case DRIVED:
        adj = pida.slice(motion[0], 30, dt);
        mr_out(gs - adj);
        ml_out(gs + adj);
        break;
////////////////////////////////////////////////////////////////////
      case WALLF_LIMIT:
      {
        int logic = (digitalRead(18) == HIGH);
        Serial.println(logic);
        logic = (logic == 1) - (logic == 0);

        int adj_l = 0.2*gs;
        mr_out(gs - logic * adj_l);
        ml_out(gs + logic * adj_l);
        break;
      }
    }
  }

//EOF