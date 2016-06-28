#ifndef CONTROL
#define CONTROL
#include "digital_edge.h"
//lf
  void stop_sensor(int sn, int mode) // 1 - stop when high, 0 - stop when low
  {    
    int logic = 1;
    async_reset();
    while(logic)
    {
      read_sv();

      if(mode == 1){logic = (svals[sn] < 50);}
      else if(mode == 0){logic = (svals[sn] > 50);}    

      async();
    }
  }

  void stop_lost_line()
  {
    async_reset();   

    while(density != 0)
    {
      read_sv();
      async();
    }
  }

  void stop_eval_line(char compare, char mask)
  {
    async_reset();

    while(!eval_line(compare, mask))
    {
      async();
    }
  }

  void stop_dd(float dist)
  {
    async_reset();
    start_encoders();

    int dir = sign_f(dist);

    while(track_distance()*dir < abs(dist)){async();}

    end_encoders();
  }

  void stop_deg(float deg)
  {
    float rad = ((deg/180.0)*3.14159265359)/2;
    async_reset();
    start_encoders();

    int dir = sign_f(rad);

    while(dir * track_angle() < abs(rad))
    {
      async();
    }

    end_encoders();
  }

//DISTANCE
  void stop_box(int mode)
  {
    async_reset();

    while(digitalRead(BOX_LIMIT_PIN) == mode)
    {
      async();
    }
  }

  void stop_prox()
  {
    async_reset();

    while(digitalRead(PROX_PIN) == HIGH)
    {
      async();
    }
  }

//INTERNAL
  void stop_time(u_long mils)
  {    
    start_count_m();
    async_reset();

    while(get_count_m() < mils)
    {
      async();
    }
  }

//EXTENDED
  void stop_corner()
  {
    read_sv();
    async_reset();
    while( density <= 2 || (svals[0] < 50 && svals[NUMLSENSORS - 1] < 50) )
    {
      if(async_state != LINEF)
        read_sv();
      async();
    }
  }

  void stop_no_corner()
  {    
    read_sv();
    async_reset();
    while(density > 3 || svals[0] > 50 || svals[NUMLSENSORS - 1] > 50)
    {
      read_sv();
      async();
    }
  }

  void stop_start_pull()
  {
    edge st(START_PIN);

    st.eval();
    while(!st.rising)
    {
      st.eval();
    }
  }

  void stop_state_rising()
  {
    async_reset();
    while(!is_state_rising())
    {
      async();
    }
  }

  void stop_red()
  {
    async_reset();
    while(1)
    {
      Serial.println(get_color());
      delay(100);

      async();
    }
  }

  void no_state()
  {
    async_state = NO_STATE;
  }

  void go()
  {
    async_reset();
    while(1){async();}
  }
#endif