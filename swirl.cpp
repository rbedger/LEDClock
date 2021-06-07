#include "prog.h"
#include "swirl.h"
#include <math.h>

Swirl::Swirl(Ledutils& ledutils)
: _ledutils(ledutils){
	int centerX = round(MatrixWidth / 2.0);
	int centerY = round(MatrixHeight / 2.0);

	_current = _indices;
	_count = 1;
}

void Swirl::handle(CRGB* leds)
{
	if (millis() - _lastExecutionTime > 100) {
		leds[*_current] = CHSV(random8(), 255, 255);
		if (++_count > NUM_LEDS) {
			_count = 1;
			_current = _indices;
		}
		else {
			_current++;
		}
		_lastExecutionTime = millis();
	}
}
