#include "asee_lib.h"
#include "sensors.h"
#include "toolbox.h"

float dd; //distance travled, used for relative distance manuvers

//UPDATE
  //VARS
    int rtickL, ltickL, r;
    float motion[3] = {0,0,0}; //angle, right motor ticks/second, left motor ticks/second

  void update(float dt)
  {
    rtickL = get_dr();
    ltickL = get_dl();
    eval_angle();

    dd += ((ltickL + rtickL)/2)*MPT;

  }

  void eval_angle()
  {
    float r = (-WB_L/2)*((rtickL + ltickL)/(rtickL - ltickL));

    if(r > 100) return;

    float da = 0;

    if(r < 0)
    {
      da = -(rtickL * MPT)/(r-WB_L/2);
    }
    else
    {
      da = -(ltickL * MPT)/(r+WB_L/2);
    }

    motion[0] += da;
  }

  void start_dist()
  {
    dd = 0;
  }

  void get_dist()
  {
    return dd;
  }

  void reset_angle()
  {
    motion[0] = 0;
  }

  float get_angle()
  {
    return motion[0];
  }
//CONTROL
	void stop_sensor(char line, char mask) // if mask = 0x00 then line is compared to density, positive means stop when greater, negitive means stop when less 
  {
    async_reset();
    logic = 1; 

    while(logic)
    {
    	if(mask == 0x00)
    	{
        int density = read_density();
    		if(line < 0){logic = (line > density);}
    		else{logic = (line < density);}
    	}
    	else
    	{
    		logic = ( (line & mask) != (read_char() & mask) );
    	}
      async();
    }
  }

  void stop_dd(float dist)
  {
    async_reset();
    int dir = sign_f(dist);
    start_dist();

    while(dd*dir < abs(get_dist())){async();}
  }

  void stop_dw(int dist, int mode) //0 - stop when under 1 - stop when over
  {
    async_reset();
    int rtdist = 0;
    int logic = 1;

    while(logic)
    {
      async();
      rtdist = get_dist_w();
      if(mode == 0){logic = (rtdist >= dist);}
      else if(mode == 1){logic = (rtdist <= dist);}
    }
  }

  void stop_dr(int dist, int mode) //0 - stop when under 1 - stop when over
  {
    async_reset();
    int rtdist = 0;
    int logic = 1;

    while(logic)
    {
      async();
      rtdist = get_dist_r();
      if(mode == 0){logic = (rtdist >= dist);}
      else if(mode == 1){logic = (rtdist <= dist);}
    }
  }

  void stop_corner()
  {
    async_reset();

    while( read_density() < 3 || eval_line(0x00, 0x81) )
    {
      async();
    }
  }

  void stop_time(int mils)
  {
    start_count();

    while(get_count() < mils)
    {
      async();
    }
  }

  void no_state()
  {
    async_state = -1;
  }

  void go()
  {
    async_reset();
    while(1){async();}
  }

  void stop_deg(float a)
  {
    async_reset();
    reset_angle();
    int dir = sign_f(a);

    while(motion[0]*dir < abs(a))
    {
      async();
    }
  }

  void stop_lost_line()
  {
    async_reset();   

    while(read_density() != 0)
    {
      async();
    }
  }

//CALIBRATE
  void calibrate_all(int mode) // 0 - new calibration, 1 - load from eeprom
  {
    if(mode == 0){
      start_counter();

      long count = 4001

      while(count < 4000)
      {
        count = get_count();

        if(count < 1000)
          dist_cal(0)
        else if(count < 2000)
          dist_cal(1)
        else if(count < 4000)
          qtr8_cal(0);
      }

      dist_cal(4);
      qtr8_cal(2);
    }
    else
    {
      qtr8_cal(1);
      dist_cal(2);
      dist_cal(3);
    }
  }
