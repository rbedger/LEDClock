// noise.h

#ifndef _NOISE_h
#define _NOISE_h

#include <FastLED.h>
#include "ledutils.h"
#include "prog.h"
#include "brightness.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Noise {
public:
	Noise(
		LedUtils &ledUtils,
		Brightness &brightness);

	void Handle(CRGB* leds);

private:
	LedUtils ledUtils;
	Brightness brightness;
	// The 32bit version of our coordinates
	uint16_t x;
	uint16_t y;
	uint16_t z;
	void FillNoise8();

	// We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
	// use the z-axis for "time".  speed determines how fast time moves forward.  Try
	// 1 for a very slow moving effect, or 60 for something that ends up looking like
	// water.
	// uint16_t speed = 1; // almost looks like a painting, moves very slowly
	uint16_t speed = 20; // a nice starting speed, mixes well with a scale of 100
	// uint16_t speed = 33;
	// uint16_t speed = 100; // wicked fast!

	// Scale determines how far apart the pixels in our noise matrix are.  Try
	// changing these values around to see how it affects the motion of the display.  The
	// higher the value of scale, the more "zoomed out" the noise iwll be.  A value
	// of 1 will be so zoomed in, you'll mostly see solid colors.

	// uint16_t scale = 1; // mostly just solid colors
	// uint16_t scale = 4011; // very zoomed out and shimmery
	uint16_t scale = 311;

	// This is the array that we keep our computed noise values in
	uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];
};

#endif

