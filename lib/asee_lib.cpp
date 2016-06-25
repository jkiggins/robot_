#include <Servo.h>
#include <Arduino.h>

int async_state = -1;

#include "QuadDecode_def.h"

#include <elapsedMillis.h>

#include "asee_lib.h"

#include "digital_edge.h"
#include "toolbox.h"
#include "sensors.h"

#include "motion.h"
#include "control.h"

#include "debug.h"

//INIT
  void init_a()
  {
    pinMode(MDR0, OUTPUT);pinMode(MDR1, OUTPUT);pinMode(MDL0, OUTPUT);pinMode(MDL1, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(WF_PIN, INPUT);
    pinMode(BOX_LIMIT_PIN, INPUT);
    pinMode(D0, INPUT_PULLUP);pinMode(D1, INPUT_PULLUP);pinMode(D2, INPUT_PULLUP);pinMode(D3, INPUT_PULLUP); //DIP SWITCHES

    deps.attach(SERVOPIN);
    deps.write(SERVOHOME);
    
    digitalWrite(13, HIGH);

    Serial.begin(9600);

    mrdecode.setup();
    mldecode.setup();
    end_encoders();

    //set_color_pins(24, 26, A16);

    last_line = 1;

    #ifdef DEBUG_COLOR
      //debug_color();
    #endif
    #ifdef DEBUG_ENCODERS
      debug_encoders();
    #endif
  }

  void lf_async(float dt)
  {
    pos = read_line();

    if(density != 0){      
      adj = pidlf.slice(CENTEROFLINE - pos, dt);
      mr_out(gs + adj);
      ml_out(gs - adj);
      digitalWrite(13, HIGH);

    }else{
      digitalWrite(13, LOW);

      mr_out(-last_line*160);
      ml_out(last_line*160);
    }
  }

  void drived_async(float dt)
  {
    mr_out(gs);
    ml_out(gs);
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

  void async_reset()
  {
    switch(async_state)
    {
      case LINEF:
        pidlf.set_pid(.49, 0, 10);
        w = 0; wsum = 0; adj = 0; pos = CENTEROFLINE;
        break;
      case LFSET:
       pidlf.set_pid(.47, 0, 11.2);
        async_state = LINEF;
        break;
      case DRIVED:
        break;
    }
  }

  void async()
  {
    switch(async_state)
    {
      case LINEF:
        lf_async(0.4);
        break;
////////////////////////////////////////////////////////////////////
      case DRIVED:
        drived_async(0.4);
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
