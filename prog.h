#ifndef __PROG_H__
#define __PROG_H__

#include <WiFiUdp.h>

#define WLED_CONNECTED (WiFi.status() == WL_CONNECTED)

#define DATA_PIN D3
#define LED_TYPE WS2812B
#define COLOR_ORDER RGB
#define DEFAULT_BRIGHTNESS 255

const uint8_t MatrixWidth = 21;
const uint8_t MatrixHeight = 5;
#define MAX_DIMENSION ((MatrixWidth>MatrixHeight) ? MatrixWidth : MatrixHeight)
#define NUM_LEDS (MatrixWidth * MatrixHeight)

#define DEBUG

#ifdef DEBUG
	#define SERIAL_PRINT(x) Serial.print(x)
	#define SERIAL_PRINTLN(x) Serial.println(x)
#else
	#define SERIAL_PRINT(x)
	#define SERIAL_PRINTLN(x)
#endif


#endif // __PROG_H__
