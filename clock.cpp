#include "clock.h"
#include "font.h"
#include "prog.h"

Clock::Clock(Ntp& ntp, Font& font)
    :   ntp(ntp),
		font(font)
{
    ledsPrevious = (CRGB*)malloc(sizeof(CRGB) * NUM_LEDS);
    ledsNew = (CRGB*)malloc(sizeof(CRGB) * NUM_LEDS);
}

void Clock::Handle(CRGB* leds) {
    time_t now = ntp.GetLocalTime();

    int hourNow = hour(now);
    int minuteNow = minute(now);

    SERIAL_PRINT("Now: ");
    SERIAL_PRINTLN(now);

    if (previousHour != hourNow || previousMinute != minuteNow) {
        // if recovering from a crash, we don't want to fade from "00:00"
        if (previousHour == 0) {
			DrawTime(ledsPrevious, hourNow, minuteNow);
        }
        else {
			DrawTime(ledsPrevious, previousHour, previousMinute);
        }

        DrawTime(ledsNew, hourNow, minuteNow);

        fade = millis() + 5000;
    }

    previousHour = hourNow;
    previousMinute = minuteNow;

    for (int i = 0; i < NUM_LEDS; i++) {
        if (fade == 0 // how?
            || millis() > fade // we've passed the time we're supposed to be fully faded
            || ledsNew[i] == CRGB(CRGB::Black) // the new color is black, just skip the fade
            )
        {
            leds[i] = ledsNew[i];
        }
        else {
			uint8_t amount = 
				255 // max amount
				- ((fade - millis()) / 5000.0) // fraction of time passed
				* 255;

			leds[i] = blend(ledsPrevious[i], ledsNew[i], amount);
        }
    }
}

void Clock::DrawTime(
    CRGB* leds,
    uint8_t hour,
    uint8_t minute)
{
    fill_solid(leds, NUM_LEDS, CRGB::Black);

	uint16_t offset = 60 * hour + minute;

    font.DrawDigit(leds, hour / 10, 2, offset);
    font.DrawDigit(leds, hour - (hour / 10) * 10, 6, offset);
    font.DrawSeparator(leds, 10, offset);
    font.DrawDigit(leds, minute / 10, 12, offset);
    font.DrawDigit(leds, minute - (minute / 10) * 10, 16, offset);
}


