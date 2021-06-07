#include "singlecolor.h"

SingleColor::SingleColor(
	Brightness& brightness
) : _brightness(brightness)
{
}

void SingleColor::handle(CRGB* leds) {
	if (millis() - _lastColorChange > 5000) {
		_lastColorChange = millis();

		fill_solid(leds, NUM_LEDS, CHSV(random8(), 255, _brightness.getBrightness()));
	}
}

