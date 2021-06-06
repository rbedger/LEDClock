#ifndef __PROG_H__
#define __PROG_H__

#include <WiFiUdp.h>

#define WLED_CONNECTED (WiFi.status() == WL_CONNECTED)

#define DEBUG

#ifdef DEBUG
	#define SERIAL_PRINT(x) Serial.print(x)
	#define SERIAL_PRINTLN(x) Serial.println(x)
#else
	#define SERIAL_PRINT(x)
	#define SERIAL_PRINTLN(x)
#endif


#endif // __PROG_H__
