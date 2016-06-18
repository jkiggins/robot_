#include <Arduino.h>

#include "G:/college/ASEE/ENR259281/final/robit_new/lib/asee_lib.h"

void setup()
{
  init_a();
  setup_color();
}

RGB color;

void loop()
{
  color = read_color();

  Serial.print("RED: ");/**/Serial.println(color.r);
  Serial.print("GREEN: ");/**/Serial.println(color.g);
  Serial.print("BLUE: ");/**/Serial.println(color.b);
  Serial.println();
  delay(100);
}

