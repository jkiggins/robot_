#include "lib/asee_lib.h"

#define TIME2BOX 500
#define TIMEBACKBOX 150
#define DIST2BOX 1.5
#define DIST2SETTLE .5
#define BASE_SPEED 120
#define SPRINT 175
#define PRECISION_SPEED 75

void setup()
{
  init_a();
  drive_d(BASE_SPEED, .7);
  add_state(0, 0, 0.0);


 ////////////////////////  
  corner_l();  
  /////////////////////////
  lf_corner(SPRINT);
  sprint(5);
  /////////////////////////
  corner_l();
  sprint(4);
  //////////////////////////
  corner_l();
  
  corner_2();
  
  /////////////////////////
  corner_r();
  lf_corner(SPRINT);
  sprint(5);
  ////////////////////////
  corner_r();
  lf_d(150, 8);
  corner_r();
  ///////////////////////  
  lf_corner(175);
  sprint(5);
  delay(100);
  corner_r();
  delay(100);
  //////////////////////
  
  lf_corner(BASE_SPEED);
  break_mots();
  delay(100);
  drive_t(BASE_SPEED, 50);
  break_mots();
  delay(100);
  turnl();
  break_mots();
  delay(100);
  lf_d(100, DIST2SETTLE);
  break_mots();
  delay(100);
  corner_l();  
  //////////////////////
  corner_l();
  /////////////////////
  lf_corner(SPRINT);
  sprint(5);
  corner_l();
  lf_corner(SPRINT);
  drive_t(SPRINT, 1000);
 

  //////////////
  break_mots();
  //////////////

}
void loop()
{
  states();
} 

void corner_l()
{
  lf_corner(BASE_SPEED);
  drive_t(BASE_SPEED, TIME2BOX);
  break_mots();
  depr();
  drive_t(-BASE_SPEED, 90);
  turnl();
  break_mots();
  drive_d(-BASE_SPEED, -.01);
  break_mots();
  lf_d(100, DIST2SETTLE);
  break_mots();
}

void corner_2()
{
  lf_corner(BASE_SPEED);
  drive_t(175, 350);
  break_mots();
  drive_t(-120, 250);
  turnl();
  lf_d(100, 1);  
}

void corner_r()
{
  lf_corner(BASE_SPEED);
  drive_t(BASE_SPEED, TIME2BOX);
  break_mots();
  depl();
  drive_t(-BASE_SPEED, 100);
  turnr();
  break_mots();
  lf_d(100, DIST2SETTLE);
}

void sprint(int distance)
{
  lf_d(SPRINT, distance);
}

