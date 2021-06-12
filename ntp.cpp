#include "prog.h"
#include "ntp.h"
#include <Timezone.h>

const char* ntpServerName = "0.us.pool.ntp.org";
Toki toki = Toki();
IPAddress ntpServerIP;
time_t localTime;
unsigned long ntpPacketSentTime = 999000000L;

TimeChangeRule daylight = {"CDT", Second, Sun, Mar, 2, -300 };
TimeChangeRule standard = {"CST", First, Sun, Nov, 2, -360 };

Timezone* centralTimezone = new Timezone(daylight, standard);

WiFiUDP ntpUdp;

void Ntp::Connect() {
	ntpConnected = ntpUdp.begin(ntpLocalPort);
}

time_t Ntp::GetLocalTime()
{
    return localTime;
}

bool Ntp::IsTimeSet() {
    return timeIsSet;
}

void Ntp::HandleTime() {

  toki.millisecond();
  toki.setTick();

  if (toki.isTick()) //true only in the first loop after a new second started
  {
      UpdateLocalTime();
  }

  if (ntpConnected && millis() - ntpLastSyncTime > (1000*NTP_SYNC_INTERVAL) && WIFI_CONNECTED)
  {
    if (millis() - ntpPacketSentTime > 10000)
    {
      SendNTPPacket();
      ntpPacketSentTime = millis();
    }
    if (CheckNTPResponse())
    {
      ntpLastSyncTime = millis();
    }
  }
}

void Ntp::SendNTPPacket()
{
  if (!ntpServerIP.fromString(ntpServerName)) //see if server is IP or domain
  {
    WiFi.hostByName(ntpServerName, ntpServerIP, 750);
  }

  SERIAL_PRINTLN("send NTP");
  byte pbuf[NTP_PACKET_SIZE];
  memset(pbuf, 0, NTP_PACKET_SIZE);

  pbuf[0] = 0b11100011;   // LI, Version, Mode
  pbuf[1] = 0;     // Stratum, or type of clock
  pbuf[2] = 6;     // Polling Interval
  pbuf[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  pbuf[12]  = 49;
  pbuf[13]  = 0x4E;
  pbuf[14]  = 49;
  pbuf[15]  = 52;

  ntpUdp.beginPacket(ntpServerIP, 123); //NTP requests are to port 123
  ntpUdp.write(pbuf, NTP_PACKET_SIZE);
  ntpUdp.endPacket();
}

bool Ntp::CheckNTPResponse()
{
  int packetSize = ntpUdp.parsePacket();
  if (!packetSize) return false;

  uint32_t ntpPacketReceivedTime = millis();

  SERIAL_PRINT("NTP recv, 1=");
  SERIAL_PRINTLN(packetSize);

  byte pbuf[NTP_PACKET_SIZE];
  ntpUdp.read(pbuf, NTP_PACKET_SIZE); // read the packet into the buffer

  Toki::Time arrived  = toki.fromNTP(pbuf + 32);
  Toki::Time departed = toki.fromNTP(pbuf + 40);
  if (departed.sec == 0) return false;
  //basic half roundtrip estimation
  uint32_t serverDelay = toki.msDifference(arrived, departed);
  uint32_t offset = (ntpPacketReceivedTime - ntpPacketSentTime - serverDelay) >> 1;
  #ifdef DEBUG
  //the time the packet departed the NTP server
  toki.printTime(departed);
  #endif

  toki.adjust(departed, offset);
  toki.setTime(departed, TOKI_TS_NTP);

  #ifdef DEBUG
  SERIAL_PRINT("Arrived: ");
  toki.printTime(arrived);
  SERIAL_PRINT("Time: ");
  toki.printTime(departed);
  SERIAL_PRINT("Roundtrip: ");
  SERIAL_PRINTLN(ntpPacketReceivedTime - ntpPacketSentTime);
  SERIAL_PRINT("Offset: ");
  SERIAL_PRINTLN(offset);
  SERIAL_PRINT("Serverdelay: ");
  SERIAL_PRINTLN(serverDelay);
  #endif

  UpdateLocalTime();
  timeIsSet = true;
  return true;
}

void Ntp::UpdateLocalTime()
{
  localTime = centralTimezone->toLocal(toki.second());
}
