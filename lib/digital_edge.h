#ifndef DIGITAL_EDGE
	#define DIGITAL_EDGE

	#include <Arduino.h>
	
	class edge
	{
	private:
		void wait_pin_settle(int pin, int mils)
		{
			value = digitalRead(pin);
			count = millis();
			int hold;
			while((millis() - count) < mils)
			{
				hold = digitalRead(pin);
				if(value != hold){
					value = hold;
					count = millis();
				}
			}
		}
	public:
		int last_value, value, pin;
		long count;
		edge(int arg_pin)
		{
			pin = arg_pin;
			last_value = -1;
		}

		int is_rising()
		{
			wait_pin_settle(pin, 10);
			int tmp = (last_value == 0 && value == 1);
			last_value = value;
			return tmp;
		}

		int is_falling()
		{
			wait_pin_settle(pin, 10);
			int tmp = (last_value == 1 && value == 0);
			last_value = value;
			return tmp;
		}

		int get_last()
		{
			return last_value;
		}
	};

	class do_once
	{
	private:
		int fc_flag;
	public:
		do_once()
		{
			this->reset();
		}

		int check()
		{
			int tmp = fc_flag;
			fc_flag = 0;
			return tmp;
		}

		void reset()
		{
			fc_flag = 1;
		}
	};
#endif
