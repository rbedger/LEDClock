#include "singlecolor.h"

SingleColor::SingleColor(
	Brightness& brightness
) : brightness(brightness)
{
}

void SingleColor::Handle(CRGB* leds) {
	if (millis() - lastColorChange > 1000) {
		lastColorChange = millis();

		fill_solid(leds, NUM_LEDS, CHSV(random8(), 255, brightness.GetBrightness()));
	}
}

