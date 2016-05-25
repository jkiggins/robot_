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
  mid_line(); 
  corner_l(); //TOP LEFT
//ODD CORNER
  lf_settle(BASE_SPEED);
  stop_time(TIME2SETTLE);
  lf(BASE_SPEED);
  corner_l(); //LEFT TOP CENTER
///////////////////////

//MIDDLE TURN
  lf_settle(BASE_SPEED);
  stop_time(TIME2SETTLE);
  lf(BASE_SPEED);
  stop_corner();
  // James Wall
  dr(200);
  stop_time(600);

  dr(-100);
  stop_time(150);

  dr(-110);
  stop_time(200);
  set_last_line(1);
  
  //set_last_line(1);
  
  line();
//////////////////////


  corner_r(); //TOP LEFT MIDDLE
  mid_line();
  corner_r(); //BOTTOM LEFT MIDDLE

  line();
  corner_r(); //BOTTOM RIGHT MIDDLE
  
  mid_line();
  corner_r(); //TOP RIGHT MIDDLE

  line();
  stop_corner();

  odd_corner();
  line();
  
  corner_l(); //TOP RIGHT CENTER

  line();
  corner_l(); //TOP RIGHT

  mid_line();

  corner_l(); //BOTTOM RIGHT
  line();

  stop_corner();

  dr(BASE_SPEED);
  stop_time(250);

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

void mid_line()
{
  lf_settle(BASE_SPEED);
  stop_time(TIME2SETTLE);
  lf(BASE_SPEED);
  stop_corner();
  stop_time(TIME2SETTLE);
}

void line()
{
  lf_settle(BASE_SPEED);
  stop_time(TIME2SETTLE);
  lf(BASE_SPEED);
}

void odd_corner()
{
  dr(-200);
  stop_time(10);
  rotate(-160, 0);
  stop_lost_line();
  set_last_line(1);
}

void loop()
{
}

