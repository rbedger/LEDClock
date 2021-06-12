// Ledutils.h

#ifndef _LEDUTILS_h
#define _LEDUTILS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class LedUtils {
	public:
		uint16_t GetXY(uint8_t x, uint8_t y);

	private:
		uint8_t GetX(uint16_t absolute);
		uint8_t GetY(uint16_t absolute);
};

#endif

