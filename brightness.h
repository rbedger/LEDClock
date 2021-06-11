// brightness.h

#ifndef _BRIGHTNESS_h
#define _BRIGHTNESS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Brightness {
	public:
		void handle();
		uint8_t getBrightness();
		bool saturated();

	private:
		unsigned int readAdc();

	private:
		uint8_t _brightness = 100;
		unsigned int _latest_adc;
};

#endif

