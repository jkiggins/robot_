#include "lib/asee_lib.h"

void setup()
{
  init_a();
  delay(500);
  calibrate(140, 1);
  
  lf(BASE_SPEED); 
  
  corner_r(); //BOTTOM right
  mid_line();     
  corner_r(); //TOP right
    
//ODD CORNER
  settle_for_time();
  lf(BASE_SPEED);
  corner_r(); //right TOP CENTER
///////////////////////

//MIDDLE TURN
  settle_for_time();
  lf(BASE_SPEED);

  break_corner();
 
  turnl();
  settle_for_time();

  corner_l(); //TOP right MIDDLE

  settle_for_time();
  
  lf(BASE_SPEED);
  avoid_corner();
  lf(BASE_SPEED);
 
  corner_l(); //BOTTOM right MIDDLEs

  rotate(-BASE_SPEED, 1);
  stop_eval_line(0x80, 0x80);
  dr(100);
  stop_time(900);
  //stop_box();

  mots_off();
}


void corner_l()
{
  break_corner();
  depl();
  turnr();
}

void corner_r()
{
  break_corner();
  depr();
  turnl();
}

void mid_line()
{
  settle_for_time();
  lf(SPRINT);
  avoid_corner();
  lf(BASE_SPEED);
  stop_time(600);
  lf(100);
}

void line()
{
  lf_settle(BASE_SPEED);
  stop_time(TIME2SETTLE);
  lf(BASE_SPEED);
}

void odd_corner()
{
  break_mots(10);
  rotate(-160, 0);
  stop_lost_line();
  set_last_line(1);
}

void turn_out()
{
  arc(22, SLOW);
  stop_box(1);
}

void avoid_corner()
{
  stop_corner();
  stop_no_corner();
}

void settle_for_time()
{
  lf_settle(SBASE_SPEED);
  stop_time(TIME2SETTLE);
}

void loop()
{
}

