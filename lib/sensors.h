
long last;
long counter;
elapsedMicros eu;

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

  void read_sv()
  {
    density = 0;
    sv_char = 0x00;

    for(int i = 0; i < NUMLSENSORS; i++)
    {
      svals[i] = (analogRead(svPins[i]) - low[i])*sv_scale[i];
      bin = (svals[i] > 30);
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
    mrdecode.write(0);

    ltickL = mldecode.read();
    mldecode.write(0);

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

/*/COLOR SENSOR
  #define COLOR_CLOCK 25
  #define COLOR_DATA_DIGI 31
  #define COLOR_DATA_ANALOG A20

  int ambient;

  void setup_color()
  {
  	//begin read, get ambient data
		pinMode(COLOR_DATA_DIGI, OUTPUT);
		digitalWrite(COLOR_DATA_DIGI, LOW);
		digitalWrite(COLOR_DATA_DIGI, HIGH);
		ambient = analogRead(COLOR_DATA_ANALOG);
  }

  void clock_color()
  {
  	digitalWrite(COLOR_CLOCK, LOW);
  	digitalWrite(COLOR_CLOCK, HIGH);
  }

	RGB read_color()
	{
		RGB ret;

		clock_color();
		ret.r = analogRead(COLOR_DATA_ANALOG) - ambient;

		clock_color();
		ret.g = analogRead(COLOR_DATA_ANALOG) - ambient;

		clock_color();
		ret.b = analogRead(COLOR_DATA_ANALOG) - ambient;

		return ret;
	}
	*/

	//TIME
		long get_dt()
		{
      long tmp = eu;
      eu = 0;
      return tmp;
		}

		void start_count()
		{
			counter = millis();
		}

		long get_count()
		{
			return (millis() - counter);
		}

		long get_abs_time()
		{
			return millis();
		}
