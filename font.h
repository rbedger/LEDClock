// #define FASTLED_ALLOW_INTERRUPTS 0

#include "FastLED.h"
#include "ledutils.h"
#include "brightness.h"

#ifndef _FONT_h
#define _FONT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Font {
	public:
		Font(
            CRGBPalette16 palette,
            LedUtils& ledutils,
            Brightness& brightness);

		void DrawDigit(
            CRGB* leds,
            uint8_t digit,
            uint8_t x_offset,
            uint16_t pallette_offset = 0);

		void DrawSeparator(
            CRGB* leds,
            uint8_t x_offset,
            uint16_t pallette_offset = 0);

	private:
		void SetPixel(
            CRGB* leds,
            uint8_t x,
            uint8_t y,
            bool on,
            uint16_t pallette_offset);

	private:
		CRGBPalette16 palette;
		LedUtils& ledUtils;
		Brightness& brightness;
};


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

#endif

