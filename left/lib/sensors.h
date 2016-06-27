#ifndef SENSORS
	#define SENSORS

elapsedMillis em;
u_long counterm;

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

    const int svPins[NUMLSENSORS] = {A0,A1,A2,A3,A4,A5,A6, A7};
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

    long ltickL, lastTickL, rtickL;
    float angle;
  	QuadDecode<1> mldecode;	// Template using FTM1 pins 3,4

    #ifdef DEBUG_ENCODERS
      #include "debug.h"
    #endif

  void start_encoders()
  {
  	mldecode.start();
  	lastTickL = 0;
  	angle = 0;
  }

  float track_distance()
  {
  	return mldecode.calcPosn()*MPT;
  }

  float track_angle()
  {
  	long lt;
  	lt = mldecode.calcPosn();
    ltickL = lt - lastTickL;
    lastTickL = lt;

    rtickL = -ltickL;

    float r = (-WB_L/2)*((rtickL + ltickL)/(rtickL - ltickL));
    float da = 0;

    if(r > 100) return angle;

    if(r < 0)
    {
      da = -(rtickL * MPT)/(r-WB_L/2);
    }
    else
    {
      da = -(ltickL * MPT)/(r+WB_L/2);
    }

    angle += da;

    return angle;
  }

  void end_encoders()
  {
  	mldecode.disable();
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

//COLOR
    int COLOR_IN = A8;

    float get_color()
    {
      return analogRead(COLOR_IN);
    }

//TIME

    u_long get_dt_m()
    {
      long tmp = em;
      em = 0;
      return tmp;
    }

		void start_count_m()
		{
			counterm = millis();
		}

		u_long get_count_m()
		{
			return (millis() - counterm);
		}

		u_long get_abs_time_m()
		{
			return millis();
		}

//BUTTONS
  edge stoggle(STATE_PIN);
  int is_state_rising()
  {
    stoggle.eval();
    return stoggle.rising;
  }

#ifdef DEBUG_LINE
  #include "debug.h"
#endif

//CALIBRATE
void calibrate()
{
  int tval;
  while(!is_state_rising())
  {
    for(int i = 0; i < NUMLSENSORS; i++)
    {
      tval = analogRead(svPins[i]);

      if(tval < low[i]) {low[i] = tval;}
      else if(tval > high[i]) {high[i] = tval;}
    }

    #ifdef DEBUG_LINE
      //debug_calibration();
    #endif
  }

  for(int i = 0; i < NUMLSENSORS; i++)
  {
    sv_scale[i] = POSSCALE/(high[i] - low[i]);
  }

  #ifdef DEBUG_LINE
    while(1){debug_line_sensor(1);}

    async_state = LINEF;
    async_reset();
    int pos;

    while(1)
    {
    	pos = read_line();
    	Serial.println(pidlf.slice(CENTEROFLINE - pos, .4));
    }
  #endif
}
#endif