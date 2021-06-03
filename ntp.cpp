#include "prog.h"
#include "ntp.h"
#include <Timezone.h>

const char* ntpServerName = "0.us.pool.ntp.org";
Toki toki = Toki();
IPAddress ntpServerIP;
time_t localTime;
unsigned long ntpPacketSentTime = 999000000L;
Timezone* centralTimezone = new Timezone(
	{Second, Sun, Mar, 2, -300},
	{First, Sun, Nov, 2, -360}
);

WiFiUDP ntpUdp;

void Ntp::connect() {
	ntpConnected = ntpUdp.begin(ntpLocalPort);
}

time_t Ntp::getLocalTime()
{
	return time_t();
}

void Ntp::handleTime() {
  handleNetworkTime();

  toki.millisecond();
  toki.setTick();

  if (toki.isTick()) //true only in the first loop after a new second started
  {
    #ifdef DEBUG
    Serial.print(F("TICK! "));
    toki.printTime(toki.getTime());

    #endif

    updateLocalTime();
  }
}

void Ntp::handleNetworkTime()
{
  if (ntpConnected && millis() - ntpLastSyncTime > (1000*NTP_SYNC_INTERVAL) && WLED_CONNECTED)
  {
    if (millis() - ntpPacketSentTime > 10000)
    {
      sendNTPPacket();
      ntpPacketSentTime = millis();
    }
    if (checkNTPResponse())
    {
      ntpLastSyncTime = millis();
    }
  }
}

void Ntp::sendNTPPacket()
{
  if (!ntpServerIP.fromString(ntpServerName)) //see if server is IP or domain
  {
    WiFi.hostByName(ntpServerName, ntpServerIP, 750);
  }

  DEBUG_PRINTLN(F("send NTP"));
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

bool Ntp::checkNTPResponse()
{
  int cb = ntpUdp.parsePacket();
  if (!cb) return false;

  uint32_t ntpPacketReceivedTime = millis();
  DEBUG_PRINT(F("NTP recv, l="));
  DEBUG_PRINTLN(cb);
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
  Serial.print("Arrived: ");
  toki.printTime(arrived);
  Serial.print("Time: ");
  toki.printTime(departed);
  Serial.print("Roundtrip: ");
  Serial.println(ntpPacketReceivedTime - ntpPacketSentTime);
  Serial.print("Offset: ");
  Serial.println(offset);
  Serial.print("Serverdelay: ");
  Serial.println(serverDelay);
  #endif

  updateLocalTime();
  return true;
}

void Ntp::updateLocalTime()
{
  unsigned long tmc = toki.second()+ utcOffsetSecs;
  localTime = centralTimezone->toLocal(tmc);
}

void Ntp::getTimeString(char* out)
{
  updateLocalTime();
  byte hr = hour(localTime);
  if (useAMPM)
  {
    if (hr > 11) hr -= 12;
    if (hr == 0) hr  = 12;
  }
  sprintf_P(out,PSTR("%i-%i-%i, %02d:%02d:%02d"),year(localTime), month(localTime), day(localTime), hr, minute(localTime), second(localTime));
  if (useAMPM)
  {
    strcat(out,(hour(localTime) > 11)? " PM":" AM");
  }
}

byte Ntp::weekdayMondayFirst()
{
  byte wd = weekday(localTime) -1;
  if (wd == 0) wd = 7;
  return wd;
}


//time from JSON and HTTP API
//void setTimeFromAPI(uint32_t timein) {
//  if (timein == 0 || timein == UINT32_MAX) return;
//  uint32_t prev = toki.second();
//  //only apply if more accurate or there is a significant difference to the "more accurate" time source
//  uint32_t diff = (timein > prev) ? timein - prev : prev - timein;
//  if (toki.getTimeSource() > TOKI_TS_JSON && diff < 60U) return;
//
//  toki.setTime(timein, TOKI_NO_MS_ACCURACY, TOKI_TS_JSON);
//  if (diff >= 60U) {
//    updateLocalTime();
//    calculateSunriseAndSunset();
//  }
//  if (presetsModifiedTime == 0) presetsModifiedTime = timein;
//}
