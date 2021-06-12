#include "prog.h"
#include "ledutils.h"

uint16_t LedUtils::GetXY(uint8_t x, uint8_t y)
{
	if (y & 0x01)
	{
		// Odd rows run backwards
		uint8_t reverseX = (MatrixWidth - 1) - x;
		return y * MatrixWidth + reverseX;
	}
	else {
		// Even rows run forwards
		return y * MatrixWidth + x;
	}
}

uint8_t LedUtils::GetX(uint16_t absolute)
{
	uint8_t x = absolute % MatrixWidth;

	// Odd rows run backwards
	if (GetY(absolute) & 0x01) {
		return MatrixWidth - 1 - x;
	}
	return x;
}

uint8_t LedUtils::GetY(uint16_t absolute)
{
	return absolute / MatrixWidth;
}