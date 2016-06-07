#include <EEPROMex.h>
#include <limits.h>
#include <Arduino.h>

#include "asee_lib.h"

//SENSOR
	void sensor::sensor(int arg_pin)
	{
		pin = arg_pin;
		lh.low = MAX_INT;
		lh.high = 0;
		scale = 1.0;
	}

	void sensor::push_cval()
	{
		if(val < lh.low){lh.low = val;}
		else if(val > lh.high){lh.high = val;}
	}

	int sensor::read()
	{
		return (analogRead(pin) - lh.low)*scale;
	}

//EEPROM
	int by_off = 0;
	void write_calibration(sensor * s, int n)
	{
		for (int i = 0; i < n; ++i)
		{
			EEPROM.writeBlock(by_off, s[i].data)
			by_off += 8;
		}		
	}

	void read_calibration(sensor * s, int n, int addr)
	{
		cdata tmp;
		for (int i = 0; i < n; ++i)
		{
			tmp = readBlock(addr + i*8, s.data);
			s[i].lh = tmp;
		}
	}

//qtr-8
  const sensor qtr8[NUMLSENSORS] = {sensor(A8),sensor(A7),sensor(A6),sensor(A3),sensor(A2),sensor(A1),sensor(A0),sensor(A17)};
  char sv_char, bin;
  int w, wsum, density;
  int svals[8];


  void read_sv(int * write_back)
  {
  	sv_char = 0xFF;
  	density = 0;

  	for (int i = 0; i < NUMLSENSORS; ++i)
  	{
  		svals[i] = qtr8[i].read();

  		bin = (svals[i] > 50);
  		density += bin;
  		sv_char &= (bin << i);
  	}
  }

  int read_line()
  {
    read_sv();
    w = 0;wsum = 0;

    for(long i = 0; i < NUMLSENSORS; i++)
    {
      w += svals[i];
      wsum += svals[i]*i*POSSCALE;
    }

    if(svals[0] > 50 && svals[NUMLSENSORS - 1] < 50){last_line = -1;}
    else if(svals[0] < 50 && svals[NUMLSENSORS - 1] > 50){last_line = 1;}

    return (wsum/w);
  }

  int read_density()
  {
    read_sv();
    return density;
  }

  char read_char()
  {
  	read_sv();
  	return sv_char;
  }

  int eval_line(char compare, char mask)
  {
    char sv_char = read_char();
    return (sv_char & mask) == (compare & mask);
  }

  void qtr8_cal(int mode);
  {  	
  	if(mode == 0){
	  	for (int i = 0; i < NUMLSENSORS; ++i)
	  	{
	  		qtr8[i].push_cval();}
	  	}
	  }else if(mode == 1){read_calibration(qtr8, NUMLSENSORS, QTR_START);}
	  else if(mode == 2){write_calibration(qtr8, NUMLSENSORS);}
  }

//DIST
  sensor dw (DWPIN);
  sensor dr (DRPIN);
  int odr = 0;

  int get_dist_w()
  {
  	return dw.read();
  }

  int get_dist_r()
  {
  	odr = dr.read();
  	return odr;
  }

  int spike_dr()
  {
  	return ((dr.read() - odr) >= DR_SPIKE);
  }

  void dist_cal(int mode)
  {
  	if(mode == 0){dw.push_cval();}
  	else if(mode == 1){dr.push_cval();}
  	else if(mode == 2){read_calibration(&dw, 1, D_START);}
  	else if(mode == 3){read_calibration(&dr, 1, D_START + 8);}
  	else if(mode == 4){write_calibration(&dw, 1, D_START);write_calibration(&dr, 1, D_START + 8);}
  }

//ENCODERS
  Encoder mldec(30,29);
	Encoder mrdec(26,27);

  int get_dr()
  {
  	int tmp = mrdec.read();
  	mrdec.write(0);
  	return tmp;
  }

  int get_dl()
  {
  	int tmp = mldec.read();
  	mldec.write(0);
  	return tmp;
  } 

//TIMING
  elapsedMillis em;
 	int overflow = 0;

 	long counters[2];

	int get_emili()
	{
		long tmp = counters[0];
		counters[0] = em;

		return em - tmp;		
	}

	float get_abs_time(int mode) // 0 - ms, 1 - s
	{
		return em/(1 + 999*mode);
	}

	int start_counter()
	{		
		counters[1] = em
	}

	//index exists and counters is an array to support multiple counters if nessisary in the future
	long get_count(int index) 
	{
		return em - counters[1];
	}