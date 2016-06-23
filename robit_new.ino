#include "lib/asee_lib.h"

void setup()
{
  init_a();
  calibrate();

  digitalWrite(RED_ENABLE, HIGH);
  //digitalWrite(BLUE_ENABLE, HIGH);

  while(1){}

  dr(200);
  stop_corner();
  dr(100);
  stop_time(200);

lf(BASE_SPEED);
corner_l(); // BOTTOM RIGHT OUT

settle_for_time();
lf(SPRINT);
avoid_corner();

corner_l(); // TOP RIGHT OUT
settle_for_time();

lf(BASE_SPEED);

  stop_corner(); // TOP MIDDLE RIGHT
  dr(100);
  stop_time(300);
  mots_off();
  depr();
  dr(-100);
  stop_time(100);
  turnl();

  settle_for_time();
  lf(BASE_SPEED);

///////////////////// MIDDLE TURN
  stop_corner();
  dr(-100);
  stop_time(50);
  turnl();
///////////////////// MIDDLE TURN

    lf_settle(120);
  stop_time(TIME2SETTLE);
  lf(120);

  corner_r(); // TOP RIGHT IN

  lf(BASE_SPEED);
  avoid_corner();
  //corner_r(); // BOTTOM RIGHT IN

  stop_corner();  
  dr(100);
  stop_time(250);
  mots_off();
  depl();
  dr(-100);
  stop_time(100);
  turnr();

  mots_off();
  delay(300);
  

  dr(-100);
  stop_time(1000);

  arc(50,100);
  stop_dd(19);
  
  depr();
  mots_off();
  delay(500);

  arc(0, -150);
  stop_time(400);

  dr(200);
  stop_dd(16);

 mots_off(); 
  
}

void loop(){}

void corner_r()
{
  stop_corner();  
  dr(100);
  stop_time(300);
  mots_off();
  depl();
  dr(-100);
  stop_time(100);
  turnr();
}

void corner_l()
{
  break_corner();
  
  dr(100);
  stop_time(250);
  mots_off();
  depr();
  delay(300);
  dr(-100);
  stop_time(150);
  turnl();
}


void settle_for_time()
{
  lf_settle(SBASE_SPEED);
  stop_time(TIME2SETTLE);
}

void off_line_box()
{
  stop_corner();
  mots_off();
  delay(200);
  dr(-100);
  stop_time(50);
  arc(-8, 100);
  stop_dd(7.6);
  mots_off();
  depr();
  delay(200);
  arc(-10, -100);
  stop_dd(-6);
  turnr();
}

void avoid_corner()
{
  stop_corner();
  stop_no_corner();
}

