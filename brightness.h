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
		void Handle();
		uint8_t GetBrightness();
		bool IsSaturated();
		const int DefaultBrightness = 100;

	private:
		unsigned int ReadADC();

	private:
		uint8_t currentBrightness = DefaultBrightness;
		unsigned int latestADCValue;
};

#endif

