#include "remote.h"
#include <FS.h>

#define FORCE_PUBLISH_INTERVAL_MS 30000

Remote::Remote(
		Modekeeper& modekeeper)
    :   _telnet_server(23),
		_web_server(80),
		_modekeeper(modekeeper)
{
}

void Remote::setup() {
    _telnet_server.begin();
    _ws_server.listen(8080);

    SPIFFS.begin();

    _web_server.onNotFound([this]() {
        if (!Remote::handleFileRead(_web_server.uri())) {
            _web_server.send(404, "text/plain", "404: Not Found");
        }
        });

    _web_server.begin();
}

bool Remote::handleFileRead(String path) {
    if (path.endsWith("/")) {
        path += "index.htm";
    }
    String contentType = "text/html";
    if (SPIFFS.exists(path)) {
        File file = SPIFFS.open(path, "r");
        size_t sent = _web_server.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;
}

void Remote::handleData(int8_t c) {
    _modekeeper.stamp();
    switch (c) {
    case '1':
        _modekeeper.setMode(Modekeeper::Mode::SNAKE);
        break;
    case '2':
        _modekeeper.setMode(Modekeeper::Mode::CLOCK);
        break;
    case '3':
        _modekeeper.setMode(Modekeeper::Mode::RAINBOW);
        break;
    case '4':
        _modekeeper.setMode(Modekeeper::Mode::SINGLECOLOR);
        break;
    case '5':
        _modekeeper.setMode(Modekeeper::Mode::BRIGHTNESS);
        break;
    case 'a':
        _modekeeper.emitEvent(Modekeeper::Event::LEFT);
        break;
    case 'd':
        _modekeeper.emitEvent(Modekeeper::Event::RIGHT);
        break;
    }
}

void Remote::onMessage(WebsocketsClient& client, WebsocketsMessage& message) {
    handleData(message.rawData()[0]);
    client.send("Echo: " + message.data());
}

void Remote::pollAllWsClients() {
    for (auto& client : _ws_clients) {
        client.poll();
    }
}

void Remote::handle() {
    // Normal TCP socket
    if (_telnet_client && _telnet_client.connected()) {
        if (_telnet_client.available() > 0) {
            int8_t c = _telnet_client.read();
            Serial.println(String(c));
            handleData(c);
        }
    }
    else {
        _telnet_client = _telnet_server.available();
        if (_telnet_client) {
            _telnet_client.setTimeout(5000); // default is 1000
        }
    }

    // Websocket
    if (_ws_server.poll()) {
        auto ws_client = _ws_server.accept();
        ws_client.onMessage([this](WebsocketsClient& client, WebsocketsMessage message) { onMessage(client, message); });
        _ws_clients.push_back(ws_client);
    }

    pollAllWsClients();

    _web_server.handleClient();
}

