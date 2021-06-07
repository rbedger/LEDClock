// singlecolor.h

#ifndef _SINGLECOLOR_h
#define _SINGLECOLOR_h

#include <FastLED.h>

#include "prog.h"
#include "brightness.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SingleColor {
public:
	SingleColor(
		Brightness& brightness
	);

	void handle(CRGB* leds);

private:
	Brightness _brightness;
	unsigned long _lastColorChange;
};

#endif

