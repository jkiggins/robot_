#include "asee_lib.h"

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

#include <Arduino.h>
#include <limits.h>
#include <Servo.h>
#include <elapsedMillis.h>
#include <EEPROMex.h>

#include "digital_edge.h"

#include "toolbox.h"
#include "sensors.h"

#ifdef DEBUG
  #include "debug.h"
#endif

#include "motion.h"
#include "control.h"



//INIT
  void init_a()
  {
    pinMode(MDR0, OUTPUT);pinMode(MDR1, OUTPUT);pinMode(MDL0, OUTPUT);pinMode(MDL1, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(WF_PIN, INPUT);
    pinMode(BOX_LIMIT_PIN, INPUT);

    deps.attach(SERVOPIN);
    deps.write(SERVOHOME);

    pinMode(29, INPUT_PULLUP);pinMode(30, INPUT_PULLUP);pinMode(31, INPUT_PULLUP);pinMode(33, INPUT_PULLUP);

    dd_flag = 0;
    digitalWrite(13, LOW);

    Serial.begin(9600);

    last_line = 1;
  }

//ASYNC
  int dt;

  void async_reset()
  {
    switch(async_state)
    {
      case LINEF:
        pidlf.set_pid(.4, 0, 10);
        w = 0; wsum = 0; adj = 0; pos = CENTEROFLINE;
        break;
      case LFSET:
        pidlf.set_pid(.6, 0, 200);
        async_state = LINEF;
      case DRIVED:
        pidlf.set_pid(1, 0, 0);
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
        pos = read_line();
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
        break;
////////////////////////////////////////////////////////////////////
      case DRIVED:
        adj = pida.slice(motion[0], dt);
        mr_out(gs - adj);
        ml_out(gs + adj);
        break;
////////////////////////////////////////////////////////////////////
      case WALLF_LIMIT:
      {
        int logic = (digitalRead(18) == HIGH);
        logic = (logic == 1) - (logic == 0);

        int adj_l = 0.2*gs;
        mr_out(gs - logic * adj_l);
        ml_out(gs + logic * adj_l);
        break;
      }
    }
  }

//EOF
