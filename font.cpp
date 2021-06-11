#include "font.h"

Font::Font(
    CRGBPalette16 palette,
    Ledutils& ledutils,
    Brightness& brightness)
:
    _palette(palette),
    _ledutils(ledutils),
	_brightness(brightness)
{
}

void Font::setPixel(CRGB* leds, uint8_t x, uint8_t y, bool on, uint16_t pallette_offset) {
    uint16_t xy = _ledutils.XY(x, y);
    if (on) {
        if (_brightness.saturated()) {
            leds[xy] = CRGB::White;
        }
        else {
            leds[xy] = ColorFromPalette(_palette, xy + pallette_offset, _brightness.getBrightness(), NOBLEND);
        }
    }
    else {
        leds[xy] = CRGB::Black;
    }
}

void Font::drawDigit(CRGB* leds, uint8_t digit, uint8_t x_offset, uint16_t pallette_offset) {
    for (byte y = 0; y < 5; y++) {
        for (byte x = 0; x < 3; x++) {
            setPixel(leds, x + x_offset, y, digits[digit][5 - y - 1][x], pallette_offset);
        }
    }
}

void Font::drawSeparator(CRGB* leds, uint8_t x_offset, uint16_t pallette_offset) {
    for (byte y = 0; y < 5; y++) {
        setPixel(leds, x_offset, y, separator[5 - y - 1], pallette_offset);
    }
}

