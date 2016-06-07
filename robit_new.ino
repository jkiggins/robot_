#include "lib/asee_lib.h"


#define TIME2BOX 500
#define TIMEBACKBOX 150
#define DIST2BOX 1.5
#define TIME2SETTLE 600
#define BASE_SPEED 255
#define SPRINT 175
#define PRECISION_SPEED 75

void setup()
{
  init_a();
  calibrate(110, 1);
  
  //calibrate(110, 1);
  
  lf(BASE_SPEED);
  corner_r(); //BOTTOM right
  mid_line(); 
  corner_r(); //TOP right
//ODD CORNER
  lf_settle(BASE_SPEED);
  stop_time(TIME2SETTLE);
  lf(BASE_SPEED);
  corner_r(); //right TOP CENTER
///////////////////////

//MIDDLE TURN
  lf_settle(BASE_SPEED);
  stop_time(TIME2SETTLE);
  lf(BASE_SPEED);
  stop_corner();

////BREAK
  dr(-255);
  stop_time(30);
////BREAK

  set_last_line(-1);

  lf_settle(BASE_SPEED);
  stop_time(TIME2SETTLE);

  
 /* // James Wall
  dr(200);
  stop_time(600);

  dr(-100);5
  stop_time(150);

  dr(-110);
  stop_time(200);
  set_last_line(1);
  
  //set_last_line(1);
  
  line();
  */
//////////////////////


  corner_l(); //TOP right MIDDLE

  lf_settle(BASE_SPEED);
  stop_time(TIME2SETTLE);
  lf(BASE_SPEED);
  stop_corner();
  stop_time(TIME2SETTLE);
  lf(BASE_SPEED);
 
  corner_l(); //BOTTOM right MIDDLE

  dr(-110);
  stop_time(200);
  turn_out();

   wf_limit(120, 0);
  //stop_box(0);
  stop_time(725);
  //stop_box(1);
  //stop_time(500);
  stop_box(1);
  dr(-110);
  stop_time(5);
  depr();

delay(150);
 
  rotate(BASE_SPEED, 1);
  stop_time(550);
  dr(200);
  stop_time(500);

  /*line();
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
  */

  break_mots();
}


void corner_l()
{
  stop_corner();
  break_mots();
  dr(110);
  stop_time(200);
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
  stop_time(200);
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

void turn_in()
{
  rotate(SLOW, 0);
  stop_deg(.35);
  arc(22, SLOW);
  stop_deg(-.2);
}

void turn_out()
{
  rotate(-BASE_SPEED, 1);
  stop_deg(-.35);
  arc(22, SLOW);
  stop_box(1);
}

void loop()
{
}

