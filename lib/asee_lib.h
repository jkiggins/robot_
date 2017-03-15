#ifndef asee_h
  #define asee_h

//MODES
  //#define INC_PID
  //s#define DEBUG

  #define PINC_PIN 33
  #define DINC_PIN 32

  #define TIME2BOX 500
  #define TIMEBACKBOX 150
  #define DIST2BOX 1.5
  #define TIME2SETTLE 500
  #define MID 2000
  #define BASE_SPEED 200
  #define SBASE_SPEED 120
  #define SPRINT 255
  #define PRECISION_SPEED 75
  #define TURN_SPEED 200

  //MISC
  #define TPM 600
  #define LAST_DIMENSION 1



  #define TPS_TO_POWER 10
  #define AD_PIN 5
  #define WF_PIN 18
  #define BOX_LIMIT_PIN 19
  #define SLOW 100



  //OTHER MISC


  #define POSSCALE 100.0
  #define MINPOWER 70
  #define HSCALE .82
  #define CENTEROFLINE 350

  //Async flags
  #define LINEF 0
  #define LFSET 1
  #define WALLF_LIMIT 2
  #define DRIVED 3
  #define NO_STATE -1

  typedef struct
  {
    int r,g,b;
  }RGB;

  void calibrate();



  //UPDATE
    void update(int dt);
    float eval_angle();

  //SPEED CONTROL
    void mr_out(int speed);
    void ml_out(int speed);

  //LF
    void lf_reset();
    void read_sv();
    int read_line();

    void lf(int s);

    void lf_settle(int s);

    void set_last_line(int ll);

    void stop_sensor(int sn, int mode); //mode: 0 - rising, 1 - falling, 2 - rise then fall
    int eval_line(char compare, char mask);
    void stop_eval_line(char compare, char mask);

  //DR
    void dr_reset();
    void dr(int s);


  //ARC
    void arc_reset();
    void rotate(int speed, int mode); //0 - both motors, 1 - right motor on, 2 - left motor on
    void arc(float r, int speed);

  //ZX
    int get_dist();
    void stop_zx(int dist, int mode);

    void wf_limit(int speed, int mode);

  //INIT
    void init_a();
    void blink_led();

  //ASYNC
    void async_reset();
    void async();

    void no_state();

  //COLOR SENSOR
    void setup_color();
    void clock_color();
    RGB read_color();

  //CONTROL
    void depr();
    void depl();

    void turnr();
    void turnl();

    void lf_d(int speed, float d);
    void lf_t(int speed, int mils);

    void stop_corner();
    void stop_no_corner();
    void stop_time(int mils);

    void break_mots(int t);
    void mots_off();
    void slow_mots(int dir_l, int dir_r, int t);
    void break_corner();

    void stop_dd(float dist);

    void stop_lost_line();

    void stop_box(int mode);

    void stop_pb();

    void go();

    int eval_dip(char compare, char mask);

    void stop_eval_dip(char compare, char mask);

  //TIME
    long get_dt();
    void start_count();
    long get_count();
    long get_abs_time();


  //toolbox
  int sign_f(float val);
  int sign_i(int val);
  void print_sv();
#endif
