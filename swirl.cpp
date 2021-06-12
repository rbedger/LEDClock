#include "prog.h"
#include "swirl.h"
#include <math.h>

Swirl::Swirl(
	LedUtils& ledutils,
	Brightness& brightness)
:	ledUtils(ledutils),
	brightness(brightness)
{
	int centerX = round(MatrixWidth / 2.0);
	int centerY = round(MatrixHeight / 2.0);

	current = indices;
	count = 1;
}

void Swirl::Handle(CRGB* leds)
{
	if (millis() - lastExecutionTime > 100) {
		leds[*current] = CHSV(random8(), 255, brightness.GetBrightness());
		if (++count > NUM_LEDS) {
			count = 1;
			current = indices;
		}
		else {
			current++;
		}
		lastExecutionTime = millis();
	}
}
