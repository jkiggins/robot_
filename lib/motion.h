#ifndef motion
  #define motion
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
	  #define MINPOWER 70
	  #define HSCALE .82
	  #define RSCALE .5 //defines turn-in radius for wall follow
	  #define OTIME 1000 //the time for turn-in osscilation

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

		void init_a();

	

	//SPEED CONTROL
	  void mr_out(int speed);
	  void ml_out(int speed);
	  void break_mots();

	//LF
	  void lf(int s);

	  void lf_settle(int s);

    void set_last_line(int ll);
    void lf_reset();

    void lf_async(float dt);
  
  //DR
    void dr_reset();
    void dr(int s);

    void dr_async(float dt);

  //ARC
    void arc_reset();
    void rotate(int speed, int mode); //0 - both motors, 1 - right motor on, 2 - left motor on
    void arc(float r, int speed);

  //WF
  	void wf(int speed, int dist, int mode);

  	void wf_async(float dt);

  //MISC
	  void depr();
	  void depl();

#endif