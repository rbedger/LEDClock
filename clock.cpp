#include "clock.h"
#include "font.h"
#include "prog.h"

Clock::Clock(Ntp& ntp, Font& font)
    :   _leds_previous((CRGB*)malloc(sizeof(CRGB)* NUM_LEDS)),
		_leds_new((CRGB*)malloc(sizeof(CRGB)* NUM_LEDS)),
		_ntp(ntp),
		_font(font)
{}

void Clock::handle(CRGB* leds) {
    time_t now = _ntp.getLocalTime();

    int hourNow = hour(now);
    int minuteNow = minute(now);

    SERIAL_PRINT("Now: ");
    SERIAL_PRINTLN(now);

    if (_prev_hour != hourNow || _prev_minute != minuteNow) {
        // if recovering from a crash, we don't want to fade from "00:00"
        if (_prev_hour == 0) {
			drawTime(_leds_previous, hourNow, minuteNow);
        }
        else {
			drawTime(_leds_previous, _prev_hour, _prev_minute);
        }

        drawTime(_leds_new, hourNow, minuteNow);

        _fade = millis() + 5000;
    }

    _prev_hour = hourNow;
    _prev_minute = minuteNow;

    for (int i = 0; i < NUM_LEDS; i++) {
        if (_fade == 0 // how?
            || millis() > _fade // we've passed the time we're supposed to be fully faded
            || _leds_new[i] == CRGB(CRGB::Black) // the new color is black, just skip the fade
            )
        {
            leds[i] = _leds_new[i];
        }
        else {
			uint8_t amount = 
				255 // max amount
				- ((_fade - millis()) / 5000.0) // fraction of time passed
				* 255;

			leds[i] = blend(_leds_previous[i], _leds_new[i], amount);
        }
    }
}

void Clock::drawTime(
    CRGB* leds,
    uint8_t hour,
    uint8_t minute)
{
    fill_solid(leds, NUM_LEDS, CRGB::Black);

	uint16_t offset = 60 * hour + minute;

    _font.drawDigit(leds, hour / 10, 2, offset);
    _font.drawDigit(leds, hour - (hour / 10) * 10, 6, offset);
    _font.drawSeparator(leds, 10, offset);
    _font.drawDigit(leds, minute / 10, 12, offset);
    _font.drawDigit(leds, minute - (minute / 10) * 10, 16, offset);
}


