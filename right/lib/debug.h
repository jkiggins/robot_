#ifndef DEBUG
	#define DEBUG
#include "sensors.h"
#include "digital_edge.h"
#include "asee_lib.h"

	void debug_line_sensor(int mode)
	{
		int pos = read_line();
		Serial.print("D: ");Serial.print(density);
		Serial.print("  POS: ");Serial.print(pos);
		
		if(mode == 0)
		{
			Serial.print("  Analog Values: ");
			print_arri(svals, NUMLSENSORS);
		}
		else
		{
			Serial.print("    Digital Values: ");
			print_char_bitwise(sv_char);
		}

		Serial.println();
	}

	void debug_dips()
	{
		Serial.print(digitalRead(29));
		Serial.print(digitalRead(30));
		Serial.print(digitalRead(31));
		Serial.print(digitalRead(33));
		Serial.println();
	}

	void debug_calibration()
	{
		Serial.print("HIGH: ");
		print_arri(high, NUMLSENSORS);

		Serial.println();

		Serial.print("LOW: ");
		print_arri(low, NUMLSENSORS);
	}

	void debug_encoders()
	{
		start_encoders();

		while(!is_state_rising())
		{
			Serial.print("LEFT: ");Serial.println(mldecode.calcPosn());
		}

		while(!is_state_rising())
		{
			Serial.print("d");Serial.println(track_distance());
		}

		while(!is_state_rising())
		{
			Serial.print("a");Serial.println(track_angle());
		}

		end_encoders();
	}

	void debug_color()
	{
		while(1){Serial.println(get_color());}
	}
#endif