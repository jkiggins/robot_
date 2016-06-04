#ifndef asee_h

  #define asee_h
  //MISC
  #define TPM 600
  #define LAST_DIMENSION 1

  //MOTOR CONTROL
  #define MDR0 7
  #define MDR1 8
  #define MDL0 10
  #define MDL1 11
  #define PWMR 5
  #define PWML 9
  #define TPS_TO_POWER 10
  #define AD_PIN 5
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
  #define DRIVED 1
  #define LFSET 2
  #define WALLF 3

  void calibrate(int s, float d);
  
  //GENERAL PID
  	class PID
  	{
  		float lt, ct;

  		public:
  			float pidd[4]; //err, last error, running integral, adjust
  			float w[3];
  			void set_pid(float p, float i, float d);
  			float slice(float err, float s, float dt);
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

  //DR
    void dr_reset();
    void dr(int s);

    void stop_dd(float dist);

  //ARC
    void arc_reset();
    void rotate(int speed, int mode); //0 - both motors, 1 - right motor on, 2 - left motor on
    void arc(float r, int speed);

    void stop_deg(float a);

  //ZX
    int get_dist();
    void stop_zx(int dist, int mode);

    void wf(int speed, int dist, int mode);
  
  //INIT
    void init_a();
    void blink_led();

  //UPDATE
    void update(float dt);
    float eval_angle();

  //ASYNC
    void async_reset();
    void async();

    void no_state();

  //CONTROL
    void depr();
    void depl();

    void turnr(int mode);
    void turnl(int mode);

    void lf_d(int speed, float d);
    void lf_t(int speed, int mils);

    void stop_corner();
    void stop_time(int mils);

    void break_mots();

    void stop_lost_line();

    void go();


  //toolbox
  int sign_f(float val);
  int sign_i(int val);
  void print_sv();
#endif
