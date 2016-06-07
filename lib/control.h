#ifndef CONTROL
  #define CONTROL

  void calibrate_all(int mode);
  
  //UPDATE POSITION
    void update();
    void eval_angle();
    float get_angle();
    void reset_angle();

  //CONTROL
    void stop_corner();
    void stop_time(int mils);
    void stop_lost_line();
    void stop_dw(int dist, int mode);
    void stop_dr(int dist, int mode);
    void stop_line_sensor(int sn, int mode); //mode: 0 - rising, 1 - falling, 2 - rise then fall
    void stop_dd(float dist);
    void stop_deg(float a);
    void go();
#endif