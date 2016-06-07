#ifndef calibration
#define calibration
	#define MAX_SENSORS 8

	typedef struct
	{
		int low, high;
		float scale;
		int pin;
	} sensor;

	class calibration
	{
		private:
			sensor sns [MAX_SENSORS];
			int eepoff // EEPROM offset in bytes
		public:
			void calibration(int argEEPOFF);
			void push_cval(int val);
			void save();
			void load();

			float get_sensor(int i);
			int read_sensor(int i);
	};

#endif


