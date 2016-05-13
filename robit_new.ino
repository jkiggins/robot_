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
  calibrate(100);
  lf(200);
  stop_sensor(7, 0);
  break_mots();
  delay(1000);
  rotate(-120, 1);
  stop_sensor(3, 0);
  break_mots();

  lf(200);
  stop_dd(7);
  stop_sensor(7, 0);
}
void loop()
{
}

