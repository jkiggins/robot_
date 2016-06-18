#include <Arduino.h>

#include "lib/asee_lib.h"

void setup()
{
  init_a();


  calibrate();

  lf(120);
  go();
}

void loop(){}
