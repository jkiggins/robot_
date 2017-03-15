#ifndef SENSORS
#define SENSORS

//PHYSICAL
#define MPT 0.0208
#define WB_L 8.962
#define NUMLSENSORS 8

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoders.h>
#include <Arduino.h>

class timeu
{
  private:
  long last, counter;
  int m;
  public:
  timeu()
  {
    last = micros();
  }
  void start_count()
  {
    counter = micros();
  }

  long get_count()
  {
    return (micros() - counter);
  }

  long get_dt()
  {
    long tmp = micros() - last;
    last = micros();
    return tmp;
  }
};

class timem
{
  private:
  long last, counter;
  int m;
  public:
    timem()
    {
      last = millis();
    }
  void start_count()
  {
    counter = millis();
  }

  long get_count()
  {
    return (millis() - counter);
  }

  long get_dt()
  {
    long tmp = millis() - last;
    last = millis();
    return tmp;
  }
};

class line
{
  private:
  int w, wsum, pos, density, last_line, pattern;
  int svals[NUMLSENSORS];
  char bin;

  const int svs[NUMLSENSORS] = {sensor(A10, 500),sensor(A0,500),sensor(A1,500),sensor(A2,500),sensor(A3,500),sensor(A6,500),sensor(A7,500),sensor(A8,500)};

  void read_sv()
  {
    density = 0;
    sv_char = 0x00;
    int tval;

    for(int i = 0; i < NUMLSENSORS; i++)
    {
      tval = svs.read();
      bin = (svals[i] > 30);
      sv_char |= (bin << (NUMLSENSORS - 1 - i));
      density += bin;
    }
  }

  void eval_pattern();

  public:
  int get_pos()
  {
    read_sv();
    w = 0;wsum = 0;

    for(int i = 0; i < NUMLSENSORS; i++)
    {
      w += svals[i];
      wsum += svals[i]*i*POSSCALE;
    }

    if(svals[0] > 40 && svals[NUMLSENSORS - 1] < 40){last_line = -1;}
    else if(svals[0] < 40 && svals[NUMLSENSORS - 1] > 40){last_line = 1;}

    return (wsum/w);
  }

  int eval_line(char compare, char mask)
  {
    read_sv();
    return (sv_char & mask) == (compare & mask);
  }

  int get_ll()
  {
    return last_line;
  }

  int is_pattern(int pattern)
  {

  }
}

class sensor
{
  private:
  int pin, type, low, high, uspan;
  float scale;
  timeu t;
  public:
  sensor(int a_type, int a_uspan)
  {
    type = a_type;
    uspan = a_uspan;
    high = 0;  low = 1025;  scale = 1.0;
  }

  void calibration_point()
  {
    int tval = analogRead(pin);
    if(tval < low){low = tval;}
    else if(tval > high){high = tval;}
    scale = 100/(high - low);
  }

  int read()
  {
    if(t.get_dt() > uspan)
    {
      if(type == 0){return digitalRead(pin);}
      else{return analogRead(pin);}
    }
  }
};

#endif
