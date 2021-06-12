#include "brightness.h"
#include "prog.h"

const int fullySaturatedValue = 400;
const double kValue = 225.0 / fullySaturatedValue;
const double mValue = 63 - 63 * kValue;

unsigned int Brightness::ReadADC() {
    int val = 0;
    for (uint8_t i = 0; i < 5; i++) {
        int a0Val = analogRead(A0);
        val += a0Val;


        delayMicroseconds(100);
    }

    val /= 5;

	SERIAL_PRINT("A0 Value: ");
	SERIAL_PRINTLN(val);

    return val;
}

void Brightness::Handle() {
    latestADCValue = ReadADC();
    currentBrightness = kValue * latestADCValue + mValue;
}

uint8_t Brightness::GetBrightness() { return currentBrightness; }

bool Brightness::IsSaturated()
{
    return latestADCValue >= fullySaturatedValue;
}

