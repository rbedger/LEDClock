#include "prog.h"
#include "ntp.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <TimeLib.h>
#include <Timezone.h>           // http://github.com/JChristensen/Timezone
#include <Streaming.h>

Ntp ntp;

void setup()
{
	Serial.begin(115200);
	Serial.println("Booting");

	WiFi.hostname("esp-clock");
	WiFi.mode(WIFI_STA);
	WiFi.begin("SSID", "Password");
	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.println("Connection failed! Rebooting");
		delay(5000);
		ESP.restart();
	}

	Serial.println("Ready");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	ntp.connect();
}

void loop()
{
	ntp.handleTime();

#ifdef DEBUG
	Serial.print("Wifi state: ");
	Serial.println(WiFi.status());

	Serial.print("NTP last sync: ");
	Serial.println(ntp.ntpLastSyncTime);
#endif

    delay(1000);
}
