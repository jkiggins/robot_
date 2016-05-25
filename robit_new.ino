#include "lib/asee_lib.h"


#define TIME2BOX 500
#define TIMEBACKBOX 150
#define DIST2BOX 1.5
#define TIME2SETTLE 800
#define BASE_SPEED 255
#define SPRINT 175
#define PRECISION_SPEED 75

void setup()
{
  init_a();
  calibrate(110, 1);
  
  lf(BASE_SPEED);
  corner_l(); //BOTTOM LEFT
  mid_line(0);
/*
  corner_l(); //TOP LEFT
//ODD CORNER
  lf_settle(BASE_SPEED);
  stop_lf_settle(.1);
  lf(BASE_SPEED);
  corner_l(); //LEFT TOP CENTER
///////////////////////

//MIDDLE TURN
  lf_settle(BASE_SPEED);
  stop_lf_settle(.1);
  lf(BASE_SPEED);
  stop_corner();
  dr(-50);
  stop_time(200);
  set_last_line(1);
  
  line();
//////////////////////


  corner_r(); //TOP LEFT MIDDLE
  mid_line(0);
  corner_r(); //BOTTOM LEFT MIDDLE

  line();
  corner_r(); //BOTTOM RIGHT MIDDLE
  
  mid_line(1);
  corner_r(); //TOP RIGHT MIDDLE

  line();
  stop_corner();

  odd_corner();
  line();
  
  corner_l(); //TOP RIGHT CENTER

  line();
  corner_l(); //TOP RIGHT

  mid_line(0);

  corner_l(); //BOTTOM RIGHT
  line();

  stop_corner();
  */

  break_mots();
}


void corner_l()
{
  stop_corner();
  break_mots();
  dr(110);
  stop_time(275);
  break_mots();
  depl();
  dr(-110);
  stop_time(200);
  set_last_line(1);
}

void corner_r()
{
  stop_corner();
  break_mots();
  dr(110);
  stop_time(275);
  break_mots();
  depr();
  dr(-110);
  stop_time(200);
  set_last_line(-1);
}

void mid_line(int t)
{
  lf_settle(BASE_SPEED);  
  stop_lf_settle(3);
  lf(BASE_SPEED);
  
  if(t == 0) stop_corner();
  else stop_corner();
  
  stop_density(3, 2);
}

void line()
{
  lf_settle(BASE_SPEED);
  stop_lf_settle(10);
  lf(BASE_SPEED);
}

void odd_corner()
{
  rotate(-160, 0);
  stop_lost_line();
  set_last_line(1);
}

void loop()
{
}

