//LF
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

  void stop_lost_line()
  {
    async_reset();   

    while(density != 0)
    {
      read_sv();
      async();
    }
  } 

//DISTANCE
  void stop_box(int mode)
  {
    async_reset();

    while(digitalRead(BOX_LIMIT_PIN) == mode)
    {
      async();
    }
  }

//INTERNAL
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

//EXTENDED
  void stop_corner()
  {
    async_reset();
    read_sv();

    while( density < 4 || (svals[0] < 50 && svals[NUMLSENSORS - 1] < 50) )
    {
      async();
    }
  }

  void stop_no_corner()
  {
    async_reset();
    read_sv();

    while( density > 4 || svals[0] > 50 || svals[NUMLSENSORS - 1] > 50 )
    {
      async();
    }
  }

  void no_state()
  {
    async_state = -1;
  }

  void turnr()
  {
    no_state();
    rotate(-BASE_SPEED, 0);
    read_sv();

    async_reset();

    while(!eval_line(0x01, 0x01))
    {
      async();
    }
  }

  void turnl()
  {
    no_state();
    rotate(BASE_SPEED, 0);
    read_sv();

    async_reset();

    while(!eval_line(0x80, 0x80))
    {
      async();
    }
  }