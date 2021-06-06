#include "modekeeper.h"
#include "remote.h"
#define FASTLED_ALLOW_INTERRUPTS 0

#include "FastLED.h"
#include "brightness.h"
#include "clock.h"
#include "ledutils.h"
#include "ntp.h"
#include "prog.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#define DATA_PIN D3
#define LED_TYPE WS2812B
#define COLOR_ORDER RGB
#define DEFAULT_BRIGHTNESS 255

const uint8_t MatrixWidth = 21;
const uint8_t MatrixHeight = 5;
#define NUM_LEDS (MatrixWidth * MatrixHeight)

Ntp _ntp;
Ledutils _ledutils(MatrixWidth, MatrixHeight);
Brightness _brightness;
Clock _clock(NUM_LEDS, _ntp, _ledutils, _brightness);

CRGB _leds[NUM_LEDS];

void setup()
{
	Serial.begin(115200);

	SERIAL_PRINTLN("Booting");

	WiFi.hostname("esp-clock");
	WiFi.mode(WIFI_STA);
	WiFi.begin("SSID", "Password");
	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		SERIAL_PRINTLN("Connection failed! Rebooting");
		delay(5000);
		ESP.restart();
	}

	SERIAL_PRINTLN("Ready");
	SERIAL_PRINT("IP address: ");
	SERIAL_PRINTLN(WiFi.localIP());

	_ntp.connect();

	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(_leds, NUM_LEDS)
		.setCorrection(TypicalLEDStrip)
		.setDither(false);
	FastLED.setBrightness(DEFAULT_BRIGHTNESS);

	FastLED.clear(true);

	CRGB* ptr = _leds;
	ptr + 21 * 2;

	fill_solid(ptr, 21, CRGB::SeaGreen);
}


void loop()
{
	_ntp.handleTime();

	_brightness.handle();
	_clock.handle(_leds);

	FastLED.show();

	SERIAL_PRINT("Wifi state: ");
	SERIAL_PRINTLN(WiFi.status());

	SERIAL_PRINT("NTP last sync: ");
	SERIAL_PRINTLN(_ntp.ntpLastSyncTime);

#ifdef DEBUG
    delay(1000);
#endif
}
