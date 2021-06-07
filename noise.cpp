#include "prog.h"
#include "noise.h"


Noise::Noise(
    Ledutils& ledutils,
    Brightness& brightness)
    :   _ledutils(ledutils),
        _brightness(brightness)
{
    // Initialize our coordinates to some random values
    _x = random16();
    _y = random16();
    _z = random16();
}

void Noise::handle(CRGB* leds) {
    static uint8_t ihue = 0;
    fillnoise8();
    for (int i = 0; i < MatrixWidth; i++) {
        for (int j = 0; j < MatrixHeight; j++) {
            // We use the value at the (i,j) coordinate in the noise
            // array for our brightness, and the flipped value from (j,i)
            // for our pixel's hue.
            leds[_ledutils.XY(i, j)] = CHSV(_noise[j][i], 255, _noise[i][j]);

            // You can also explore other ways to constrain the hue used, like below
            // leds[XY(i,j)] = CHSV(ihue + (noise[j][i]>>2),255,noise[i][j]);
        }
    }
    ihue += 1;
}

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void Noise::fillnoise8() {
    for (int i = 0; i < MAX_DIMENSION; i++) {
        int ioffset = _scale * i;
        for (int j = 0; j < MAX_DIMENSION; j++) {
            int joffset = _scale * j;
            _noise[i][j] = inoise8(_x + ioffset, _y + joffset, _z);
        }
    }
    _z += _speed;
}
