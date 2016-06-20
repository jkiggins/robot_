elapsedMillis em;
elapsedMicros eu;
u_long counteru, counterm;

//Motion and position VARS
float motion[3] = {0,0,0}; //angle, right motor ticks/second, left motor ticks/second

float dd; //distance travled, used for relative distance manuvers
int dd_flag; //when set to 1 dd is incremented with encoders

//Calibration VARS
float sv_scale[8];
int low[8] = {1025, 1025, 1025, 1025, 1025, 1025,1025, 1025};
int high[8] = {0, 0, 0, 0, 0, 0, 0, 0};

//LF ################################
  //VARS
    PID pidlf;
    int svals[8];
    char sv_char;

    int w, wsum, pos, density;
    char bin;
    int last_line;

    const int svPins[NUMLSENSORS] = {A10,A0,A1,A2,A3,A6,A7,A8};
		//const int svPins[NUMLSENSORS] = {A8,A7,A6,A3,A2,A1,A0,A10};

  void read_sv()
  {
    density = 0;
    sv_char = 0x00;

    for(int i = 0; i < NUMLSENSORS; i++)
    {
    	svals[i] = 0;
      svals[i] += analogRead(svPins[i]);
      svals[i] += analogRead(svPins[i]);
      svals[i] += analogRead(svPins[i]);
      svals[i] += analogRead(svPins[i]);

      svals[i] = ((svals[i]/4) - low[i])*sv_scale[i];

      bin = (svals[i] > 50);
      sv_char |= (bin << (NUMLSENSORS - 1 - i));
      density += bin;
    }
  }

  int read_line()
  {
    read_sv();
    w = 0;wsum = 0;

    for(int i = 0; i < NUMLSENSORS; i++)
    {
      w += svals[i];
      wsum += svals[i]*i*POSSCALE;
    }

    if(svals[0] > 50 && svals[NUMLSENSORS - 1] < 50){last_line = -1;}
    else if(svals[0] < 50 && svals[NUMLSENSORS - 1] > 50){last_line = 1;}

    return (wsum/w);
  }

  void set_last_line(int ll)
  {
    last_line = ll;
  }

  int eval_line(char compare, char mask)
  {
    read_sv();
    return (sv_char & mask) == (compare & mask);
  }

//UPDATE
  //VARS
    long rtickL, ltickL;
    Encoder mldecode(32,25);
  	Encoder mrdecode(4,3);

  void update(int dt)
  {
    rtickL = mrdecode.read();
    if(rtickL != 0) {mrdecode.write(0);}

    ltickL = mldecode.read();
    if(ltickL != 0) {mldecode.write(0);}

    motion[1] = 1000000*(rtickL/dt);
    motion[2] = 1000000*(ltickL/dt);

    if(dd_flag == 1)
    {
      dd += ((ltickL + rtickL)/2)*MPT;
    }

    if(rtickL != ltickL)
    {
      motion[0] += eval_angle();
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

  int eval_dip(char compare, char mask)
  {
  	//pins 29, 30 ,31 ,33
  	char sw = 0x00;
  	mask &= 0x0F;

  	sw |= (digitalRead(33) << 0);
  	sw |= (digitalRead(31) << 1);
  	sw |= (digitalRead(30) << 2);
  	sw |= (digitalRead(29) << 3);

  	return (sw & mask) == (compare & mask);
  }

	//TIME
		u_long get_dt_u()
		{
      long tmp = eu;
      eu = 0;
      return tmp;
		}

    u_long get_dt_m()
    {
      long tmp = em;
      em = 0;
      return tmp;
    }

    void start_count_u()
		{
      counteru = micros();
    }

		void start_count_m()
		{
			counterm = millis();
		}

		u_long get_count_m()
		{
			return (millis() - counterm);
		}

    u_long get_count_u()
    {
      return (micros() - counteru);
    }

		u_long get_abs_time_m()
		{
			return millis();
		}

#ifdef DEBUG
  #include "debug.h"
#endif

//CALIBRATE
void calibrate()
{
  int tval;
  while(eval_dip(0xFF, 0x0F))
  {
    for(int i = 0; i < NUMLSENSORS; i++)
    {
      tval = analogRead(svPins[i]);

      if(tval < low[i]) {low[i] = tval;}
      else if(tval > high[i]) {high[i] = tval;}
    }

    #ifdef DEBUG
      debug_calibration();
    #endif
  }

  for(int i = 0; i < NUMLSENSORS; i++)
  {
    sv_scale[i] = POSSCALE/(high[i] - low[i]);
  }

  #ifdef DEBUG
    while(1){debug_line_sensor(0);}
    delay(100);
  #endif
}
