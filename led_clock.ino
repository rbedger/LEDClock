#define FASTLED_INTERRUPT_RETRY_COUNT 1

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
#ifdef DEBUG
	Serial.println("Booting");
#endif

	WiFi.hostname("esp-clock");
	WiFi.mode(WIFI_STA);
	WiFi.begin("SSID", "Password");
	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
#ifdef DEBUG
		Serial.println("Connection failed! Rebooting");
#endif
		delay(5000);
		ESP.restart();
	}

#ifdef DEBUG
	Serial.println("Ready");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
#endif

	_ntp.connect();

	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(_leds, NUM_LEDS)
		.setCorrection(TypicalLEDStrip)
		.setDither(false);
	FastLED.setBrightness(DEFAULT_BRIGHTNESS);

	FastLED.clear(true);
}


void loop()
{
	_ntp.handleTime();

#ifdef DEBUG
	Serial.print("Wifi state: ");
	Serial.println(WiFi.status());

	Serial.print("NTP last sync: ");
	Serial.println(_ntp.ntpLastSyncTime);
#endif

	_clock.handle(_leds);

	FastLED.show();


#ifdef DEBUG
    delay(1000);
#endif
}
