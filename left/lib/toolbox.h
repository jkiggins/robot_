#include <Arduino.h>

  int sign_f(float val)
  {
    if(val < 0){return -1;}
    else if(val > 0){return 1;}
    else{return 0;}
  }

  int sign_i(int val)
  {
    if(val < 0){return -1;}
    else if(val > 0){return 1;}
    else{return 0;}
  }

  void blink_led()
  {
    digitalWrite(13, HIGH);
    delay(300);
    digitalWrite(13, LOW);
    delay(300);
  }

void print_arrf(float * arr, int n)
{
  for (int i = 0; i < n; ++i)
  {
    Serial.print(arr[i]);
    if(i != (n-1))
      Serial.print(",");
  }
}

void print_arri(int * arr, int n)
{
  for (int i = 0; i < n; ++i)
  {
    Serial.print(arr[i]);
    if(i != (n-1))
      Serial.print(",");
  }
}

void print_char_bitwise(char ch)
{
  for (int i = 8; i >= 0; --i)
  {
    Serial.print((int)(ch >> i) & 0x01);
  }
}

void blink_async(int mode)
{
  if(mode == 0){start_count_m();digitalWrite(13, HIGH);}
  else if(get_count_m() < 500){digitalWrite(13, LOW);}
}

/*at this point, its faster and simpler to just average 4 readings, especially since my loop time for line following is at most 30us
int low-pass-data[LOW_PASS_WIDTH];

void reset_low_pass()
{
  memset(low-pass-data, -1, sizeof(low-pass-data));
}

void low_pass(int data)
{
  int avg;
  int i;

  for(i = LOW_PASS_WIDTH - 1; i > 0; i--)
  {
    low-pass-data[i] = low-pass-data[i-1];
  }
  low-pass-data[0] = data;

  for (i = 0; i < LOW_PASS_WIDTH; i++)
  {
    if(low-pass-data[i] != -1)
      avg += low-pass-data[i]
    else
      i = LOW_PASS_WIDTH;
  }

  return avg /= i;

}
*/

