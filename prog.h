#ifndef __PROG_H__
#define __PROG_H__

#include <WiFiUdp.h>

#define DEBUG

#ifdef DEBUG
	#define DEBUG_PRINT(x)
	#define DEBUG_PRINTLN(x)
	#define DEBUG_PRINTF(x)
#endif

#define WLED_CONNECTED (WiFi.status() == WL_CONNECTED)


#endif // __PROG_H__
