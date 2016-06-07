  void stop_box(int mode)
  {
    async_reset();

    while(digitalRead(BOX_LIMIT_PIN) == mode)
    {
      async();
    }
  }

  void stop_time(int mils)
  {
    async_reset();
    int dt = 0;

    while(dt < mils)
    {
      dt += em;
      async();
    }
  }

  void stop_corner()
  {
    async_reset();
    read_sv();

    while( density < 4 || (svals[0] < 50 && svals[NUMLSENSORS - 1] < 50) )
    {
      async();
    }
  }
  void stop_deg(float a)
  {
    async_reset();
    motion[0] = 0;
    int dir = sign_f(a);

    while(motion[0]*dir < abs(a))
    {
      async();
    }
  }

  void stop_lost_line()
  {
    async_reset();   

    while(density != 0)
    {
      read_sv();
      async();
    }
  }

  void stop_dd(float dist)
  {
    dd = 0;
    dd_flag = 1;
    async_reset();

    int dir = sign_f(dist);

    while(dd*dir < abs(dist)){async();}
  }

  void stop_sensor(int sn, int mode) // 0 - stop when high, 1 - stop when low
  {
    async_reset();
    int logic = 1;

    while(logic)
    {
      read_sv();

      if(mode == 0){logic = (svals[sn] < 50);}
      else if(mode == 1){logic = (svals[sn] > 50);}    

      async();
    }
  }