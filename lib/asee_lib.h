#ifndef asee_h
  #define asee_h

  #define TIME2BOX 500
  #define TIMEBACKBOX 150
  #define DIST2BOX 1.5
  #define TIME2SETTLE 800
  #define MID 2000
  #define BASE_SPEED 255
  #define SBASE_SPEED 120
  #define SPRINT 255
  #define PRECISION_SPEED 75
  #define TURN_SPEED 200

  //MISC
  #define TPM 600
  #define LAST_DIMENSION 1

  //MOTOR CONTROL
  #define MDL0 7
  #define MDL1 8
  #define MDR0 10
  #define MDR1 11
  #define PWML 5
  #define PWMR 9
  #define TPS_TO_POWER 10
  #define AD_PIN 5
  #define WF_PIN 18
  #define BOX_LIMIT_PIN 19
  #define SLOW 100

  //PHYSICAL
  #define MPT 0.0208
  #define WB_L 8.962
  
  //OTHER MISC
  #define MAX_ANGLE 3.2
  #define SERVOPIN 23
  #define SERVOHOME 90
  #define NUMLSENSORS 8
  #define POSSCALE 100.0
  #define MINPOWER 70
  #define HSCALE .82
  #define CENTEROFLINE 350

  //Async flags
  #define LINEF 0
  #define LFSET 1
  #define WALLF_LIMIT 2
  #define NO_STATE -1

  typedef struct 
  {
    int r,g,b;
  }RGB; 

  void calibrate(int s, float d);
  
  //GENERAL PID
  	class PID
  	{
  		float lt, ct;

  		public:
  			float pidd[4]; //err, last error, running integral, adjust
  			float w[3];
  			void set_pid(float p, float i, float d);
  			float slice(float err, float dt);
  	};

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

    void stop_lost_line();

    void stop_box(int mode);

    void go();

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
