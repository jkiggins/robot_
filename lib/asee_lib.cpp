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

//INIT
  void init_a()
  {
    pinMode(MDR0, OUTPUT);pinMode(MDR1, OUTPUT);pinMode(MDL0, OUTPUT);pinMode(MDL1, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(WF_PIN, INPUT);
    pinMode(BOX_LIMIT_PIN, INPUT);
    pinMode(RED_ENABLE, OUTPUT);pinMode(BLUE_ENABLE, OUTPUT); //COLOR SENSOR
    pinMode(D0, INPUT_PULLUP);pinMode(D1, INPUT_PULLUP);pinMode(D2, INPUT_PULLUP);pinMode(D3, INPUT_PULLUP); //DIP SWITCHES

    digitalWrite(RED_ENABLE, LOW);digitalWrite(BLUE_ENABLE, LOW);

    deps.attach(SERVOPIN);
    deps.write(SERVOHOME);
    
    digitalWrite(13, HIGH);

    Serial.begin(9600);

    mrdecode.setup();
    //mldecode.setup();
    end_encoders();

    last_line = 1;

    #ifdef DEBUG_ENCODERS
      debug_encoders();
    #endif
  }

  void lf_async(float dt)
  {
    
    if(density != 0){
      pos = read_line();
      adj = pidlf.slice(CENTEROFLINE - pos, dt);
      mr_out(gs + adj);
      ml_out(gs - adj);

    }else{
       
      #if defined(DEBUG) || defined(INC_PID)
        pos = read_line();
        mr_out(0);
        ml_out(0);
        pidlf.inc_pid();
      #elif defined(SMOOTH)
        adj = pidlf.slice(CENTEROFLINE - pos, dt);
        mr_out(gs + adj);
        ml_out(gs - adj);
      #else
        mr_out(-last_line*160);
        ml_out(last_line*160);
      #endif
       read_sv();
    }
  }

  void drived_async(float dt)
  {
    adj = pida.slice(track_angle(), dt);
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
  float dt;

  void async_reset()
  {
    switch(async_state)
    {
      case LINEF:
        pidlf.set_pid(.27, 0, 12.5);
        w = 0; wsum = 0; adj = 0; pos = CENTEROFLINE;
        #ifdef INC_PID
          pidlf.set_pid(.35, 0, 25);
        #endif
        break;
      case LFSET:
        pidlf.set_pid(.27, 0, 12.5);
        async_state = LINEF;
        break;
      case DRIVED:
        pida.set_pid(1, 0, 0);
        start_encoders();
        adj = 0;
        break;
    }
    get_dt_u();
  }

  void async()
  {
    dt = .4; //((float)get_dt_u())/1000.0;

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
