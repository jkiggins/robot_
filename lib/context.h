#ifndef CONTEXT
#define CONTEXT

#include "sensors.h"
#include <Encoders.h>

class context
{
  private:
  float motion[3] = {0,0,0}; //angle, right motor ticks/second, left motor ticks/second
  float dd, wb; //distance travled, used for relative distance manuvers
  int dd_flag; //when set to 1 dd is incremented with encoders
  long ltickL, rtickL;
  timeu t;

  Encoder mldecode(32,25);
  Encoder mrdecode(4,3);

  void update(int dt)
  {
    rtickL = mrdecode.read();
    mrdecode.write(0);

    ltickL = mldecode.read();
    mldecode.write(0);

    if(dd_flag == 1)
    {
      dd += ((ltickL + rtickL)/2)*MPT;
    }

    if(rtickL != ltickL)
    {
      motion[0] += this->eval_angle();
    }
  }

  float eval_angle()
  {
    float r = (-WB_L/2)*((rtickL + ltickL)/(rtickL - ltickL));
    float da = 0;

    if(r > 100) return 0;

    if(r < 0)
    {
      da = -(rtickL * MPT)/(r-WB_L/2);
    }
    else
    {
      da = -(ltickL * MPT)/(r+WB_L/2);
    }

    return da;
  }

  public:
  context(float a_wb)
  {
    wb = wb_a;
  }

  void start_dist()
  {
    dd = 0;
    dd_flag = 1;
    mldecode.write(0);
    mrdecode.write(0);
  }

  float get_dist()
  {
    return dd;
  }

};

#endif
