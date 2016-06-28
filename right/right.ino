#include "lib/asee_lib.h"

void setup()
{

  init_a();
  //RIGHT INIT
  pinMode(PROX_PIN, INPUT);
  //
  calibrate();
  stop_state_rising();
  //FIND THE LINE
  dr(150);/**/avoid_corner();/**/stop_dd(.8);mots_off();
  arc(-5, 100);/**/stop_sensor(0, 1);
  arc(5, 100);/**/stop_dd(3);
  //////////

  lf(BASE_SPEED);
  
  corner_l(); //BOTTOM RIGHT
  settle_for_time();

  lf(SPRINT);
  avoid_corner();
  corner_l(); //TOP RIGHT
  settle_for_time();

  lf(BASE_SPEED);
  corner_l(); //TOP MIDDLE RIGHT

  //////MID TURN
  lf(SBASE_SPEED);/**/stop_corner();
  dr(0);/**/stop_no_corner();
  dr(-200);
  stop_time(100);
  turnl();
  settle_for_time();
  /////MID TURN

  lf(BASE_SPEED);
  
  corner_r(); //TOP RIGHT MID AREA
  settle_for_time();

  lf(SPRINT);/**/avoid_corner();

  corner_r();
  
  //BOTTOM RIGHT MID AREA
//  stop_corner();  
//  dr(200);
//  stop_time(75);
//  mots_off();
//  depl();
//  dr(-100);
//  stop_corner();
//  dr(100);
//  stop_dd(1);
//  mots_off();
//  rotate(-100, 0);
//  stop_sensor(5, 1);
//  mots_off();

//  //OFF LINE BOX
//  dr(-100);
//  stop_time(1000);
//
//  arc(40,100);
//  stop_dd(19);
//  stop_prox();
//  dr(-100);
//  stop_dd(-.3);
//  mots_off();
//  
//  depr();//BOTTOM MIDDLE BOX
//  /////
//
//  //GET BACK
//  arc(-15, 100);
//  stop_sensor(2, 1);

  
  lf(BASE_SPEED);
  corner_r();
  lf(SPRINT);
  avoid_corner();
  corner_r();




  
  lf(BASE_SPEED);
  stop_corner();
  dr(-100);
  stop_time(100);

  rotate(100, 0);
  stop_time(500);
  turnl();
  lf(BASE_SPEED);
  corner_l();
  lf(BASE_SPEED);
  corner_l();
  lf(SPRINT);
  avoid_corner();
  corner_l();
  lf(SPRINT);
  avoid_corner();

  dr(100);
  stop_time(600);


  mots_off();
}

void loop(){}

