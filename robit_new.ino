#include "lib/asee_lib.h"

void setup()
{

  init_a();
  mr_out(100);
  ml_out(100);
  while(1){}
  calibrate();
  
  stop_dip(1, D0);
/*
  dr(200);
  avoid_corner();

  arc(-5, 100);
  stop_time(1);

  dr(100);
  stop_sensor(3, 0);

*/

  lf(BASE_SPEED);
  go();
  corner_l(); // BOTTOM RIGHT OUT
  
  settle_for_time();

  lf(SPRINT);
  avoid_corner();
  
  corner_l(); // TOP RIGHT OUT
  settle_for_time();
    /*
  
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

    lf_settle(150);
  stop_time(TIME2SETTLE);
  lf(175);

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
  mots_off();
  delay(500);
  
  depr();
  mots_off();
  delay(500);

  arc(0, -150);
  stop_time(400);

  dr(-150);
  stop_time(500);

  dr(200);
  stop_dd(16);
  */

 mots_off();
  
}

void loop(){}

void corner_r()
{
  stop_corner();  
  dr(100);
  stop_time(TIME2BOX);
  mots_off();
  
  depl();
  
  dr(-100);
  stop_time(100);
  
  turnr();
}

void corner_l()
{
  stop_corner();  
  dr(100);
  stop_time(TIME2BOX);
  mots_off();
  
  depr();
  
  dr(-100);
  stop_time(150);
  
  turnl();
}


void settle_for_time()
{
  lf(SBASE_SPEED);
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

