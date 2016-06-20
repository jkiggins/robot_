#include "lib/asee_lib.h"

void setup()
{
  init_a();

  calibrate();

  lf(BASE_SPEED);
  corner_r();
  settle_for_time();
  mots_off();
  
  
}

void loop(){}

void corner_r()
{
  break_corner();
  depl();
  turnr();
}

void settle_for_time()
{
  lf_settle(SBASE_SPEED);
  stop_time(TIME2SETTLE);
}

