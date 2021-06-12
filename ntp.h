#ifndef __NTP_H__
#define __NTP_H__

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include "Toki.h"

#define NTP_SYNC_INTERVAL 42000UL //Get fresh NTP time about twice per day
#define NTP_PACKET_SIZE 48


static const uint16_t ntpLocalPort = 2390;

class Ntp {
	public:
		void Connect();
		void HandleTime();
		time_t GetLocalTime();
		bool IsTimeSet();

		bool ntpConnected;
		unsigned long ntpLastSyncTime = 999000000L;

	private:
		void SendNTPPacket();
		bool CheckNTPResponse();
		void UpdateLocalTime();

		bool timeIsSet = false;
};
#endif // __NTP_H__
