#include "prog.h"
#include "noise.h"


Noise::Noise(
    LedUtils& ledutils,
    Brightness& brightness)
    :   ledUtils(ledutils),
        brightness(brightness)
{
    // Initialize our coordinates to some random values
    x = random16();
    y = random16();
    z = random16();
}

void Noise::Handle(CRGB* leds) {
    static uint8_t ihue = 0;

    FillNoise8();

    for (int i = 0; i < MatrixWidth; i++) {
        for (int j = 0; j < MatrixHeight; j++) {
            // We use the value at the (i,j) coordinate in the noise
            // array for our brightness, and the flipped value from (j,i)
            // for our pixel's hue.
            // leds[_ledutils.XY(i, j)] = CHSV(_noise[j][i], 255, _noise[i][j]);
            leds[ledUtils.GetXY(i, j)] = CHSV(noise[j][i], 255, brightness.GetBrightness());

            // You can also explore other ways to constrain the hue used, like below
            // leds[XY(i,j)] = CHSV(ihue + (noise[j][i]>>2),255,noise[i][j]);
        }
    }

    ihue += 1;
}

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void Noise::FillNoise8() {
    for (int i = 0; i < MAX_DIMENSION; i++) {
        int iOffset = scale * i;
        for (int j = 0; j < MAX_DIMENSION; j++) {
            int jOffset = scale * j;
            noise[i][j] = inoise8(x + iOffset, y + jOffset, z);
        }
    }
    z += speed;
}
