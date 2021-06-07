#include "brightness.h"
#include "font.h"
#include "ledutils.h"
#include "ntp.h"

#ifndef _CLOCK_h
#define _CLOCK_h

class Clock {
	public:
		Clock(uint16_t num_leds, Ntp& ntp, Ledutils &ledutils, Brightness &brightness);
		void handle(CRGB* leds);

	private:
		void setPixel(CRGB *leds, uint8_t x, uint8_t y, bool on);
		void drawDigit(CRGB *leds, uint8_t digit, uint8_t x_offset);
		void drawSeparator(CRGB *leds, uint8_t x_offset);

	private:
		uint16_t _num_leds;
		int8_t _prev_day = 0;
		int8_t _prev_hour = 0;
		int8_t _prev_minute = 0;
		unsigned long _fade = 0;
		CRGB *_leds_previous;
		CRGB *_leds_new;
		Ntp &_ntp;
		Font _font;
		Brightness& _brightness;
};

#endif

