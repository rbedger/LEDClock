#include "brightness.h"
#include "font.h"
#include "ledutils.h"
#include "ntp.h"

#ifndef _CLOCK_h
#define _CLOCK_h

class Clock {
	public:
		Clock(Ntp& ntp, Font& font);

		void Handle(CRGB* leds);

	private:
		void DrawTime(
			CRGB* leds,
			uint8_t hour,
			uint8_t minute);

	private:
		int8_t previousHour = 0;
		int8_t previousMinute = 0;
		unsigned long fade = 0;
		CRGB *ledsPrevious;
		CRGB *ledsNew;
		Ntp &ntp;
		Font font;
};

#endif

