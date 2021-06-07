// wifi_config.h

#ifndef _WIFI_CONFIG_h
#define _WIFI_CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

const char* WiFiSSID = "<SSID>";
const char* WiFiPassword = "<Password>";

#endif

