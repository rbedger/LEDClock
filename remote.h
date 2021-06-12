#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <vector>
#include "modekeeper.h"

#ifndef __REMOTE_H__
#define __REMOTE_H__

using namespace websockets;

class Remote {
public:
	Remote(Modekeeper& modekeeper);
	void Setup();
	void Handle();

private:
	void HandleData(int8_t c);
	void OnMessage(WebsocketsClient& client, WebsocketsMessage& message);
	void PollAllWsClients();
	bool HandleFileRead(String path);

private:
	WebsocketsServer websocketServer;
	std::vector<WebsocketsClient> websocketClients;

	ESP8266WebServer webServer;

	WiFiClient wifiClient;
	unsigned long retryToConnectAtMS = 0;

	Modekeeper& modekeeper;
};

#endif //__REMOTE_H__

