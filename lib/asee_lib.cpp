#include <Servo.h>
#include <Arduino.h>

#define ENCODER_OPTOMIZED_INTERRUPTS
#include <Encoder.h>

#include <elapsedMillis.h>

#include "asee_lib.h"

#include "digital_edge.h"
#include "toolbox.h"
#include "sensors.h"



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

  void lf_async(int dt)
  {
    pos = read_line();
    if(density != 0){
      adj = pidlf.slice(CENTEROFLINE - pos, dt);
      mr_out(gs + adj);
      ml_out(gs - adj);

    }else{
       
      #if defined(DEBUG) || defined(INC_PID)
        mr_out(0);
        ml_out(0);
      #else
        mr_out(-last_line*160);
        ml_out(last_line*160);
      #endif
    }

    #ifdef INC_PID
      pidlf.inc_pid();
    #endif
  }

  void drived_async(u_long dt)
  {
    adj = pida.slice(motion[0], dt);
    mr_out(gs - adj);
    ml_out(gs + adj);
  }

  void wallf_limit_async()
  {
    int logic = (digitalRead(18) == HIGH);
    logic = (logic == 1) - (logic == 0);

    int adj_l = 0.2*gs;
    mr_out(gs - logic * adj_l);
    ml_out(gs + logic * adj_l);
  }

//ASYNC
  int dt;

  void async_reset()
  {
    switch(async_state)
    {
      case LINEF:
        pidlf.set_pid(.3, 0, 15000);
        w = 0; wsum = 0; adj = 0; pos = CENTEROFLINE;
        #ifdef INC_PID
          pidlf.set_pid(.25, 0, 15000);
        #endif
        break;
      case LFSET:
        pidlf.set_pid(.3, 0, 30000);
        async_state = LINEF;
      case DRIVED:
        pidlf.set_pid(1, 0, 0);
        dd = 0;motion[0] = 0; adj = 0;dd_flag = 1;
        break;
    }
    get_dt_u();
  }

  void async()
  {
    dt = get_dt_u();
    update(dt);

    switch(async_state)
    {
      case LINEF:
        lf_async(dt);
        break;
////////////////////////////////////////////////////////////////////
      case DRIVED:
        drived_async(dt);
        break;
////////////////////////////////////////////////////////////////////
      case WALLF_LIMIT:
      {
        wallf_limit_async();
        break;
      }
    }
  }


  //EOF
