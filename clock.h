#include "brightness.h"
#include "font.h"
#include "ledutils.h"
#include "ntp.h"

#ifndef _CLOCK_h
#define _CLOCK_h

class Clock {
	public:
		Clock(Ntp& ntp, Font& font);

		void handle(CRGB* leds);

	private:
		void drawTime(
			CRGB* leds,
			uint8_t hour,
			uint8_t minute);

	private:
		int8_t _prev_day = 0;
		int8_t _prev_hour = 0;
		int8_t _prev_minute = 0;
		unsigned long _fade = 0;
		CRGB *_leds_previous;
		CRGB *_leds_new;
		Ntp &_ntp;
		Font _font;
};

#endif

