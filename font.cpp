#include "font.h"

Font::Font(
    CRGBPalette16 palette,
    LedUtils& ledutils,
    Brightness& brightness)
:
    palette(palette),
    ledUtils(ledutils),
	brightness(brightness)
{
}

void Font::SetPixel(CRGB* leds, uint8_t x, uint8_t y, bool on, uint16_t pallette_offset) {
    uint16_t xy = ledUtils.GetXY(x, y);
    if (on) {
        if (brightness.IsSaturated()) {
            leds[xy] = CRGB::White;
        }
        else {
            leds[xy] = ColorFromPalette(palette, xy + pallette_offset, brightness.GetBrightness(), NOBLEND);
        }
    }
    else {
        leds[xy] = CRGB::Black;
    }
}

void Font::DrawDigit(CRGB* leds, uint8_t digit, uint8_t x_offset, uint16_t pallette_offset) {
    for (byte y = 0; y < 5; y++) {
        for (byte x = 0; x < 3; x++) {
            SetPixel(leds, x + x_offset, y, digits[digit][5 - y - 1][x], pallette_offset);
        }
    }
}

void Font::DrawSeparator(CRGB* leds, uint8_t x_offset, uint16_t pallette_offset) {
    for (byte y = 0; y < 5; y++) {
        SetPixel(leds, x_offset, y, separator[5 - y - 1], pallette_offset);
    }
}

