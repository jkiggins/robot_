#include "lib/asee_lib.h"

void setup()
{
  init_a();

  //INIT LEFT
  pinMode(START_PIN, INPUT_PULLUP);
  //
  
  calibrate();

  stop_start_pull();

///////FINDING THE LINE
  dr(150);
  avoid_corner();
  settle_for_time();
///////

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
  arc(6, 100);
  stop_time(650);
  dr(150);
  stop_time(1000);
  
  
  mots_off(); 
  
}

void loop(){}


