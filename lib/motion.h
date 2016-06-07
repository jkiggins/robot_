//PID ################################
  void PID::set_pid(float p, float i, float d)
  {
    w[0] = p;w[1] = i;w[2] = d;
    pidd[0] = 0; pidd[1] = 0; pidd[2] = 0;
  }

  float PID::slice(float err, float s, float dtl)
  {
    if(dtl != 0)
    {
      pidd[1] = pidd[0];
      pidd[0] = err;
      pidd[2] += ((pidd[0] + pidd[1])/2)*dtl;

      pidd[2] = constrain(pidd[2], -s/w[1], s/w[1]);

      pidd[3] = pidd[0]*w[0] + pidd[2]*w[1] + (pidd[0] - pidd[1])/dtl * w[2];
    }

    return pidd[3];
  }
  
//LF ################################
  //VARS
    PID pidlf;
    long svals[8];
    char sv_char;
    long w, wsum, pos, density, last_line, bin;
    const int svPins[NUMLSENSORS] = {A8,A7,A6,A3,A2,A1,A0,A17};

  void read_sv()
  {
    density = 0;
    sv_char = 0xFF;

    for(int i = 0; i < NUMLSENSORS; i++)
    {
      svals[i] = (analogRead(svPins[i]) - low[i])*sv_scale[i];
      bin = svals[i] > 50;
      sv_char &= (bin << i);
      density += bin;
    }
  }

  int read_line()
  {
    read_sv();
    w = 0;wsum = 0;

    for(long i = 0; i < NUMLSENSORS; i++)
    {
      w += svals[i];
      wsum += svals[i]*i*POSSCALE;
    }

    if(svals[0] > 50 && svals[NUMLSENSORS - 1] < 50){last_line = -1;}
    else if(svals[0] < 50 && svals[NUMLSENSORS - 1] > 50){last_line = 1;}

    return (wsum/w);
  }

  void lf(int s)
  {
    async_state = LINEF;
    gs = s;
  }

  void lf_settle(int s)
  {
    async_state = LFSET;
    gs = s;
  }

  void set_last_line(int ll)
  {
    last_line = ll;
  }

  int eval_line(char compare, char mask)
  {
    read_sv();

    return ((compare & mask) == (sv_char & mask));
  }

//DR ################################
  //vars
    PID pida;
    float err;

  void dr(int s)
  {
    async_state = DRIVED;
    gs = s;
  }

//ARC
  void rotate(int s, int mode) //0 - both motors, 1 - right motor on, 2 - left motor on
  {
    async_state = -1;
    if(mode == 0)
    {
      mr_out(s);
      ml_out(-s);
    }
    else if(mode == 1)
    {
      mr_out(s);
      ml_out(0);
    }
    else
    {
      ml_out(-s);
      mr_out(0);
    }
  }

  void arc(float r, int s)
  {
    async_state = -1;
    mr_out( s*(1-(WB_L/(2*r))) );
    ml_out( s*(1+(WB_L/(2*r))) );
  }