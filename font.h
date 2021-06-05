#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
#include "ledutils.h"

#ifndef _FONT_h
#define _FONT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


// clang-format off
const uint8_t digits[10][5][3] = {
  {
    {1, 1, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1}
  },
  {
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1}
  },
  {
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
    {1, 0, 0},
    {1, 1, 1}
  },
  {
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1}
  },
  {
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 1},
    {0, 0, 1}
  },
  {
    {1, 1, 1},
    {1, 0, 0},
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1}
  },
  {
    {1, 1, 1},
    {1, 0, 0},
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1}
  },
  {
    {1, 1, 1},
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1}
  },
  {
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1}
  },
  {
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 1},
    {0, 0, 1}
  }
};

const uint8_t separator[5] = {
  0,
  1,
  0,
  1,
  0
};
// clang-format on

class Font {
public:
    Font(CRGBPalette16 palette, Ledutils& ledutils);
    void drawDigit(CRGB* leds, uint8_t digit, uint8_t x_offset, uint16_t pallette_offset = 0);
    void drawSeparator(CRGB* leds, uint8_t x_offset, uint16_t pallette_offset = 0);
    void setBrightness(uint8_t brightness);
    void useFullWhiteInsteadOfPalette(bool use_white);

private:
    void setPixel(CRGB* leds, uint8_t x, uint8_t y, bool on, uint16_t pallette_offset);

private:
    CRGBPalette16 _palette;
    Ledutils& _ledutils;
    uint8_t _brightness = 0;
    uint16_t _time_offset = 0;
    bool _white_instead_of_palette = false;
};

#endif

