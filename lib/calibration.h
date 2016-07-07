#ifndef calibration
#define calibration

#include <Arduino.h>

	typedef struct
	{
		int low, high;
		float scale;
		int pin;
		int type;
	} sensor;

	class sensor_array
	{
		private:
			sensor * sns;
			int eeprom_origin;
			static int eeprom_off;
		public:
			sensor_array(int);
			void push_cval(int val);
			void save();
			void load();

			float get_sensor(int i);
			int read_sensor(int i);
	};

	sensor_array::sensor_array(int len)
	{
		sns = new sensor[len];
	}

	int sensor_array::read_sensor(int i)
	{
		return (analogRead(sns[i].pin) - sns[i].low)*sns[i].scale;
	}

	sensor& sensor_array::get_sensor(int i)
	{
		return &sns[i];
	}


#endif
