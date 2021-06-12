// #define FASTLED_ALLOW_INTERRUPTS 0

#include <ESP8266WiFi.h>
#include <FastLED.h>

#include "prog.h"
#include "noise.h"
#include "modekeeper.h"
#include "remote.h"
#include "brightness.h"
#include "clock.h"
#include "ledutils.h"
#include "ntp.h"
#include "singlecolor.h"
#include "wifi_config.h"
#include "swirl.h"

Ntp _ntp;
LedUtils _ledutils;
Brightness _brightness;
Font _font(RainbowColors_p, _ledutils, _brightness);
Clock _clock(_ntp, _font);
Modekeeper _modekeeper(Modekeeper::Mode::Clock);
Remote _remote(_modekeeper);
Noise _noise(_ledutils, _brightness);
SingleColor _singleColor(_brightness);
Swirl _swirl(_ledutils, _brightness);

CRGB _leds[NUM_LEDS];
int _last_button_state = LOW;

void setup()
{
	Serial.begin(115200);

	SERIAL_PRINTLN("Booting");

	WiFi.mode(WIFI_STA);

	WiFi.hostname("LEDClock");
	WiFi.begin(WiFiSSID, WiFiPassword);
	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		SERIAL_PRINTLN("Connection failed! Rebooting");
		delay(5000);
		ESP.restart();
	}

	SERIAL_PRINTLN("Ready");

	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	_ntp.Connect();
	// _remote.setup();

	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(_leds, NUM_LEDS)
		.setCorrection(TypicalLEDStrip)
		.setDither(false);
	FastLED.setBrightness(DEFAULT_BRIGHTNESS);
}


void loop()
{
	int currentButtonState = digitalRead(BUTTON_PIN);
	if (currentButtonState == HIGH && _last_button_state == LOW) {
		_modekeeper.NextMode();
	}
	_last_button_state = currentButtonState;

	_ntp.HandleTime();

	_brightness.Handle();
	// _remote.handle();

	int currentHour = hour(_ntp.GetLocalTime());
	if (currentHour > 22 || currentHour < 8) {
		FastLED.clear(true);
		return;
	}

	switch (_modekeeper.GetMode())
	{
	case Modekeeper::Mode::Clock:
		if (!_ntp.IsTimeSet()) {
			// don't update any LEDs if the time isn't set.
			// this is especially nice if we are recovering from a crash.
			return;
		}
		_clock.Handle(_leds);
		break;
	case Modekeeper::Mode::SingleColor:
		_singleColor.Handle(_leds);
		break;
	case Modekeeper::Mode::Noise:
		_noise.Handle(_leds);
		break;
	case Modekeeper::Mode::Swirl:
		_swirl.Handle(_leds);
		break;
	}

	FastLED.show();

	SERIAL_PRINT("Wifi state: ");
	SERIAL_PRINTLN(WiFi.status());

	SERIAL_PRINT("NTP last sync: ");
	SERIAL_PRINTLN(_ntp.ntpLastSyncTime);

#ifdef DEBUG_DELAY
	delay(1000);
#endif
}
