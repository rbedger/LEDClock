// singlecolor.h

#ifndef _SINGLECOLOR_h
#define _SINGLECOLOR_h

#include "prog.h"
#include <FastLED.h>

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SingleColor {
public:
	void handle(CRGB* leds);

private:
	unsigned long _lastColorChange;
};

#endif

