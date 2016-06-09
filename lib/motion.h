int gs = 0; //global speed to be shared by all async methods
int gd = 0; //global distance away from the wall to use when wall following
int gm = 0; //global mode variable to be used by any method
int gadj = 0;
int async_state = -1;
float adj; //global pid adjust variable

Servo deps; //deposite servo variable

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

//DR ################################
  //vars
    PID pida;
    float err;

  void dr(int s)
  {
    mr_out(s);
    ml_out(s);
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

//WF
  PID pidwf;

  void wf_limit(int speed, int mode)
  {
    gs = speed;
    gm = mode;
    async_state = WALLF_LIMIT;
  }

  void break_mots()
  {
    mr_out(0);
    ml_out(0);
  }

  void slow_mots(int dir_r, int dir_l)
  {
    mr_out(-255*dir_r);
    ml_out(-255*dir_l);
  }

  void mr_out(int pwr)
  {
    pwr = constrain(pwr, -255, 255);

    if(pwr > 0)
    {
      digitalWrite(MDR0, LOW);
      digitalWrite(MDR1, HIGH);
    }
    else if(pwr == 0)
    {
      digitalWrite(MDR0, HIGH);
      digitalWrite(MDR1, HIGH);
    }
    else
    {
      digitalWrite(MDR0, HIGH);
      digitalWrite(MDR1, LOW);
    }

    analogWrite(PWMR, abs(pwr));
  }

  void ml_out(int pwr)
  {
    pwr = constrain(pwr, -255, 255);

    if(pwr > 0)
    {
      digitalWrite(MDL0, LOW);
      digitalWrite(MDL1, HIGH);
    }
    else if(pwr == 0)
    {
      digitalWrite(MDL0, HIGH);
      digitalWrite(MDL1, HIGH);
    }
    else
    {
      digitalWrite(MDL0, HIGH);
      digitalWrite(MDL1, LOW);
    }

    analogWrite(PWML, abs(pwr));
  }

  void depr()
  {
    deps.write(SERVOHOME - 45);
    delay(250);
    deps.write(SERVOHOME);
  }

  void depl()
  {
    deps.write(SERVOHOME + 45);
    delay(250);
    deps.write(SERVOHOME);
  }

  void go()
  {
    async_reset();
    while(1){async();}
  }

void calibrate(int s, float d)
{
  int tval;

  start_count();

  mr_out(100);
  ml_out(100);

  while(get_count() < 800)
  {
    for(int i = 0; i < NUMLSENSORS; i++)
    {
      tval = analogRead(svPins[i]);

      if(tval < low[i]) {low[i] = tval;}
      else if(tval > high[i]) {high[i] = tval;}
    }
  }

  break_mots();

  for(int i = 0; i < NUMLSENSORS; i++)
  {
    sv_scale[i] = POSSCALE/(high[i] - low[i]);
  }
}