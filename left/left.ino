#include "lib/asee_lib.h"

void setup()
{
  init_a();

  //INIT LEFT
  pinMode(START_PIN, INPUT_PULLUP);
  //
  
  calibrate();

  lf(BASE_SPEED);
  stop_corner();
  mots_off();

  stop_start_pull();

  //FIND THE LINE
  dr(150);/**/avoid_corner();/**/stop_dd(.8);mots_off();
  arc(-5, 100);/**/stop_sensor(0, 1);
  arc(5, 100);/**/stop_dd(3);
  //////////

  lf(BASE_SPEED);
  corner_r(); // BOTTOM LEFT OUT
  settle_for_time();

  lf(SPRINT);
  avoid_corner();


  lf(BASE_SPEED);
  corner_r(); // TOP LEFT OUT
  settle_for_time();

  lf(BASE_SPEED);

  odd_corner(); //TOP MIDDLE LEFT
  

  settle_for_time();
   
  lf(BASE_SPEED);

///////////////////// MIDDLE TURN
  off_line_box();
  settle_for_time();
///////////////////// MIDDLE TURN
  
  lf(BASE_SPEED);
  corner_l(); // TOP LEFT IN
  settle_for_time();
  
  lf(BASE_SPEED);
  avoid_corner();
  
  corner_l(); // BOTTOM LEFT IN
  settle_for_time();
  
  lf(BASE_SPEED);
  stop_time(500);
  arc(4.1, 100);
  stop_time(1200);
  mots_off(); 
  
}

void loop(){}


