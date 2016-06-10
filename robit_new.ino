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
  //corner_r(); //right TOP CENTER

  break_corner();
  dr(100);
  stop_time(200);
  depr();
  dr(-100);
  stop_time(150);
  turnl();
///////////////////////

//MIDDLE TURN
  settle_for_time();
  lf(BASE_SPEED);

  break_corner();
   dr(100);
  stop_time(200);
  dr(-100);
  stop_time(250);
 
  turnl();
  settle_for_time();

  corner_l(); //TOP right MIDDLE

  

  settle_for_time();
  
  lf(BASE_SPEED);
  avoid_corner();
  lf(BASE_SPEED);
 
  corner_l(); //BOTTOM right MIDDLEs

  rotate(-150, 0);
  stop_eval_line(0x01, 0x01);
  slow_mots(1, -1, 12);
  dr(100);
  stop_time(900);
  stop_box(1);
  dr(-100);
  stop_time(750);
  dr(80);
  stop_box(1);
  dr(-150);
  stop_time(50);
  depr();

  arc(-1, BASE_SPEED);
  stop_time(200);
  dr(200);
  stop_time(100);
  arc(-1, BASE_SPEED);
  stop_time(150);
   dr(200);
  stop_time(1000);

  mots_off();
}


void corner_l()
{
  break_corner();
  dr(100);
  stop_time(200);
  depl();
  turnr();
}

void corner_r()
{
  break_corner();
  dr(100);
  stop_time(200);
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
  lf(BASE_SPEED);
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
  lf_settle(BASE_SPEED);
  stop_time(TIME2SETTLE);
}

void loop()
{
}

