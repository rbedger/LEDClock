#include "clock.h"
#include "font.h"
#include "ledutils.h"
#include "prog.h"

Clock::Clock(uint16_t num_leds, Ntp& ntp, Ledutils& ledutils, Brightness& brightness)
    :   _num_leds(num_leds),
		_leds_previous((CRGB*)malloc(sizeof(CRGB)* _num_leds)),
		_leds_new((CRGB*)malloc(sizeof(CRGB)* _num_leds)),
		_ntp(ntp),
		_font(Font(RainbowColors_p, ledutils)),
		_brightness(brightness)
{}

void Clock::handle(CRGB* leds) {
    _font.setBrightness(_brightness.getBrightness());
    _font.useFullWhiteInsteadOfPalette(_brightness.saturated());

    time_t now = _ntp.getLocalTime();

    int hourNow = hour(now);
    int minuteNow = minute(now);

#ifdef DEBUG_CLOCK
    Serial.print("Now: ");
    Serial.println(now);

    Serial.print("Hour: ");
    Serial.println(hourNow);

    Serial.print("Minute: ");
    Serial.println(minuteNow);
#endif

    if (_prev_hour != hourNow || _prev_minute != minuteNow) {
        uint16_t prev_time_offset = 60 * _prev_hour + _prev_minute;
        _font.drawDigit(_leds_previous, _prev_hour / 10, 2, prev_time_offset);
        _font.drawDigit(_leds_previous, _prev_hour - (_prev_hour / 10) * 10, 6, prev_time_offset);
        _font.drawSeparator(_leds_previous, 10, prev_time_offset);
        _font.drawDigit(_leds_previous, _prev_minute / 10, 12, prev_time_offset);
        _font.drawDigit(_leds_previous, _prev_minute - (_prev_minute / 10) * 10, 16, prev_time_offset);

        uint16_t time_offset = 60 * hourNow + minuteNow;
        _font.drawDigit(_leds_new, hourNow / 10, 2, time_offset);
        _font.drawDigit(_leds_new, hourNow - (hourNow / 10) * 10, 6, time_offset);
        _font.drawSeparator(_leds_new, 10, time_offset);
        _font.drawDigit(_leds_new, minuteNow / 10, 12, time_offset);
        _font.drawDigit(_leds_new, minuteNow - (minuteNow / 10) * 10, 16, time_offset);
        _fade = millis() + 5000;
    }
    _prev_hour = hourNow;
    _prev_minute = minuteNow;

    for (int i = 0; i < _num_leds; i++) {
        uint8_t amount = 255 - ((_fade - millis()) / 5000.0) * 255;
        if (_fade == 0 || millis() > _fade) {
            amount = 255;
        }
        leds[i] = blend(_leds_previous[i], _leds_new[i], amount);
    }
}


