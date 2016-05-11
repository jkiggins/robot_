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

  //PHYSICAL
  #define MPT 0.0208
  #define WB_L 8.962
  
  //OTHER MISC
  #define MAX_ANGLE 3.2
  #define SERVOPIN 23
  #define SERVOHOME 90
  #define NUMLSENSORS 8
  #define POSSCALE 100
  #define MINPOWER 70

  //STATES
  #define LFCORNER 1
  #define LFD 2
  #define LFLL 3
  #define SROT 4
  #define STPLIN 5
  #define STPANGL 6
  #define BREAK 7
  #define RVSONIC 8
  #define DEPL 9
  #define DEPR 10
  #define DRIVED 11
  #define DRIVET 12 
  #define DRVZX 13
  #define LFZX 14
  #define LFCT 15

  void calibrate();
  
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
    int lost_line();
    void lf_slice(int s, float dt);

  //DR
    void dr_reset();
    int dr_slice(float d, float s, float dt);

  //ARC
    void arc_reset();
    void rotate(float deg, int speed, int mode); //0 - both motors, 1 - right motor on, 2 - left motor on
    void arc(float r, float s, int speed);

  //ZX
    int get_dist();
  
  //INIT
    void init_a();

  //UPDATE
    void update(float dt);
    float eval_angle();

  //CONTROL
    void depr();
    void depl();

    void turnr(int mode);
    void turnl(int mode);

    void lf_d(int speed, float d);
    void lf_t(int speed, int mils);

    void dr_zx(int speed, int dist);
    void lf_zx(int speed, int zx);

    void rotate_to_sensor(int speed, int pin, int mode);
    void rotate_to_pos(int speed, int pos);

    void break_mots();


  //toolbox
  int sign_f(float val);
  int sign_i(int val);
  void print_sv();
#endif
