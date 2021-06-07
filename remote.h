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
	void setup();
	void handle();

private:
	void handleData(int8_t c);
	void onMessage(WebsocketsClient& client, WebsocketsMessage& message);
	void pollAllWsClients();
	bool handleFileRead(String path);

private:
	WebsocketsServer _ws_server;
	std::vector<WebsocketsClient> _ws_clients;

	ESP8266WebServer _web_server;

	WiFiClient _wifi_client;
	unsigned long _retry_to_connect_at_ms = 0;

	Modekeeper& _modekeeper;
};

#endif //__REMOTE_H__

