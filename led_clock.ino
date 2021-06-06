#include "singlecolor.h"
#define FASTLED_ALLOW_INTERRUPTS 0

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include <FastLED.h>
#include "noise.h"
#include "modekeeper.h"
#include "remote.h"
#include "brightness.h"
#include "clock.h"
#include "ledutils.h"
#include "ntp.h"
#include "prog.h"

Ntp _ntp;
Ledutils _ledutils(MatrixWidth, MatrixHeight);
Brightness _brightness;
Clock _clock(NUM_LEDS, _ntp, _ledutils, _brightness);
Modekeeper _modekeeper(Modekeeper::Mode::CLOCK);
Remote _remote(_modekeeper);
Noise _noise(_ledutils);
SingleColor _singleColor;

CRGB _leds[NUM_LEDS];

void setup()
{
	Serial.begin(115200);

	SERIAL_PRINTLN("Booting");

	WiFi.mode(WIFI_STA);

	WiFi.hostname("LEDClock");
	WiFi.begin("SSID", "Password");
	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		SERIAL_PRINTLN("Connection failed! Rebooting");
		delay(5000);
		ESP.restart();
	}

	SERIAL_PRINTLN("Ready");

	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	_ntp.connect();
	_remote.setup();

	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(_leds, NUM_LEDS)
		.setCorrection(TypicalLEDStrip)
		.setDither(false);
	FastLED.setBrightness(DEFAULT_BRIGHTNESS);

	FastLED.clear(true);

	CRGB* ptr = _leds;
	ptr = ptr + 21 * 2;

	fill_solid(ptr, 21, CRGB::SeaGreen);
	FastLED.show();

	delay(2000);
}


void loop()
{
	if (digitalRead(BUTTON_PIN) == HIGH) {
		_modekeeper.nextMode();
	}
	_ntp.handleTime();

	_brightness.handle();
	_remote.handle();

	switch (_modekeeper.getMode())
	{
	case Modekeeper::Mode::CLOCK:
		_clock.handle(_leds);
		break;
	case Modekeeper::Mode::SINGLECOLOR:
		_singleColor.handle(_leds);
		break;
	case Modekeeper::Mode::NOISE:
		_noise.handle(_leds);
		break;
	}

	FastLED.show();

	SERIAL_PRINT("Wifi state: ");
	SERIAL_PRINTLN(WiFi.status());

	SERIAL_PRINT("NTP last sync: ");
	SERIAL_PRINTLN(_ntp.ntpLastSyncTime);

#ifdef DEBUG
	delay(1000);
#endif
}
