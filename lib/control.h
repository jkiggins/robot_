//LF
  void stop_sensor(int sn, int mode) // 0 - stop when high, 1 - stop when low
  {    
    int logic = 1;
    async_reset();
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

  void stop_eval_line(char compare, char mask)
  {
    async_reset();

    while(!eval_line(compare, mask))
    {
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
    start_count();
    async_reset();

    while(get_count() < mils)
    {
      async();
    }
  }

//EXTENDED
  void stop_corner()
  {
    read_sv();
    async_reset();
    while( density <= 3 || (svals[0] < 50 && svals[NUMLSENSORS - 1] < 50) )
    {
      read_sv();
      async();
    }
  }

  void stop_no_corner()
  {    
    read_sv();
    async_reset();
    while(density > 3 || svals[0] > 50 || svals[NUMLSENSORS - 1] > 50)
    {
      read_sv();
      async();
    }
  }

  void no_state()
  {
    async_state = NO_STATE;
  }