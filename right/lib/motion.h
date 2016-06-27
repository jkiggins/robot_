#ifndef MOTION
  #define MOTION

int gs = 0; //global speed to be shared by all async methods
//int gd = 0; //global distance away from the wall to use when wall following
int gm = 0; //global mode variable to be used by any method
int gadj = 0;
float adj; //global pid adjust variable

Servo deps; //deposite servo variable

#ifdef INC_PID
  edge p_inc(D0);
  edge d_inc(D1);
  edge p_adj(D2);
  edge d_adj(D3);
  int pv, dv;
  float D_P = .05;
  float D_D = 2;
  int count;
  int last_err = 32000;
  int err_track;
#endif

//PID ################################
void PID::set_pid(float p, float i, float d)
{
  w[0] = p;w[1] = i;w[2] = d;
  pidd[0] = 0; pidd[1] = 0; pidd[2] = 0;
}

float PID::slice(float err, float dtl)
{
  pidd[1] = pidd[0];
  pidd[0] = err;
  pidd[2] += ((pidd[0] + pidd[1])/2)*dtl;
  pidd[3] = pidd[0]*w[0] + pidd[2]*w[1] + (pidd[0] - pidd[1])/dtl * w[2];

  return pidd[3];
}

void PID::inc_pid()
{
  #ifdef INC_PID
    p_inc.eval();d_inc.eval();
    p_adj.eval();d_adj.eval();

    if(p_inc.toggle)
    {
      w[0] += D_P;
    }
    else if(d_inc.toggle)
    {
      w[2] += D_D;
    }
    else if(p_adj.toggle)
    {
      if(p_inc.state)
        D_P *= 2;
      else
        D_P /= 2;
    }
    else if(d_adj.toggle)
    {
      if(d_inc.state)
        D_D *= 2;
      else
        D_D /= 2;
    }

    if(count % 500)
    {
      Serial.print("P: ");Serial.println(w[0]);
      Serial.print("D: ");Serial.println(w[2]);
      Serial.println();
    }
    count++;
  #endif
}


//LF
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
    async_state = DRIVED;
    gs = s;
  }

//ARC
void rotate(int s, int mode) //0 - both motors, 1 - right motor on, 2 - left motor on
{
  no_state();
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
  no_state();
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

void break_mots(int t)
{
  no_state();
  stop_time(t);
  mr_out(0);
  ml_out(0);
}

void mots_off()
{
  mr_out(0);
  ml_out(0);
}

void slow_mots(int dir_l, int dir_r, int t)
{
  mr_out(-255*dir_r);
  ml_out(-255*dir_l);
  no_state();
  stop_time(t);
  mots_off();
}

void mr_out(int pwr)
{
  pwr = constrain(pwr, -255, 255);

  if(pwr > 0)
  {
    digitalWrite(MDR0, LOW);
    digitalWrite(MDR1, HIGH);
  }
  else
  {
    digitalWrite(MDR0, HIGH);
    digitalWrite(MDR1, LOW);
  }

  #ifndef NO_MOTORS
    analogWrite(PWMR, abs(pwr));
  #endif
}

void ml_out(int pwr)
{
  pwr = constrain(pwr, -255, 255);
  if(pwr > 0)
  {
    digitalWrite(MDL0, LOW);
    digitalWrite(MDL1, HIGH);
  }
  else
  {
    digitalWrite(MDL0, HIGH);
    digitalWrite(MDL1, LOW);
  }

  #ifndef NO_MOTORS
    analogWrite(PWML, abs(pwr));
  #endif
}

void depr()
{
  deps.write(SERVOHOME - 20);
  delay(100);
  deps.write(SERVOHOME - 45);
  delay(250);
  deps.write(SERVOHOME);
}

void depl()
{
  deps.write(SERVOHOME + 20);
  delay(100);
  deps.write(SERVOHOME + 45);
  delay(250);
  deps.write(SERVOHOME);
}

void turnr()
{
  mr_out(-TURN_SPEED);
  ml_out(TURN_SPEED);
  read_sv();

  while(svals[NUMLSENSORS -1] < 50){read_sv();}
  while(density == 0){read_sv();}

  mr_out(TURN_SPEED);
  ml_out(-TURN_SPEED);
  delay(75);
  mots_off();
  delay(100); 

  set_last_line(1);
}

void turnl()
{
  mr_out(TURN_SPEED);
  ml_out(-TURN_SPEED);
  read_sv();
 
  while(svals[0] < 50){read_sv();}
  while(density == 0){read_sv();}

  mr_out(-TURN_SPEED);
  ml_out(TURN_SPEED);
  delay(75);
  mots_off();
  delay(100);
  
  set_last_line(-1);
}

void break_corner()
{

}
#endif
