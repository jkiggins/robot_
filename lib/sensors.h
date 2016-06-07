#ifndef sensors
  #define sensors

	typedef struct
	{
		int low, high;
	}cdata;

	class sensor
	{
	public:

		cdata lh;
		int pin;
		float scale;
		void sensor(int arg_pin);
		void push_cvalue();
		int read();
	};

	//EEPROM
		void write_calibration(sensor * s, int n);
		void read_calibration(sensor * s, int n, int addr); 

  //QRT-8
    #define NUMLSENSORS 8
    #define POSSCALE 100.0
    #define CENTEROFLINE 350
		#define QRT_START 0

    void read_sv(int * write_back); //writes all sensor values and density, write_back must be a 10 element integer array
    int read_line(); //returns the posotion of the robot on the line
    void read_density();
    void read_char();
    void qtr8_cal(int mode);
    int eval_line(char compare, char mask);

    void stop_line(char line, char mask);

  //DISTANCE
    #define D_START 64
    #define DWPIN 19
    #define DRPIN 18
    #define DR_SPIKE 15
    int get_dist_w(); //wall distance sensor
    int get_dist_r(); //ramp distance sensor

    int spike_r(); //return 1 if distance spike

    void dist_cal();

  //ENCODERS
    #define MR_ENCA 26
    #define MR_ENCB 27
    #define ML_ENCA 30
    #define ML_ENCB 29

  	int get_dr();
  	int get_dl();

  //TIMING
  	float get_emili();
  	float get_abs_time();
  	int start_count();
  	long get_count(int index);
#endif
