void corner_r()
{
  stop_corner();  
  dr(100);
  stop_time(TIME2BOX);
  mots_off();
  depl();
  dr(-100);
  stop_time(TIMEBACKBOX);
  turnr();
}

void corner_l()
{
  stop_corner(); 
  dr(100);
  stop_time(TIME2BOX);
  depr();
  dr(-100);
  stop_time(TIMEBACKBOX);
  turnl();
}

void odd_corner()
{
  stop_corner(); // TOP MIDDLE LEFT 
  dr(0);
  stop_time(260);
  mots_off();
  depl();
  dr(-100);
  stop_time(375);
  turnr();
}


void settle_for_time()
{
  lf_settle(SBASE_SPEED);
  stop_time(TIME2SETTLE);
}

void off_line_box()
{
  stop_corner();
  dr(100);
  stop_no_corner();
  dr(-100);
  stop_corner();
  arc(-13.3, 100);
  stop_dd(4.8);
  mots_off();
  depr();
  delay(200);
  arc(-10, -100);
  stop_dd(-3);
  turnr();
}

void avoid_corner()
{
  stop_corner();
  stop_dd(3);
}

