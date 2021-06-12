#include "remote.h"
Remote::Remote(
		Modekeeper& modekeeper)
    :   webServer(80),
		modekeeper(modekeeper)
{
}

void Remote::Setup() {
    websocketServer.listen(8080);

    SPIFFS.begin();

    webServer.onNotFound([this]() {
        if (!Remote::HandleFileRead(webServer.uri())) {
            webServer.send(404, "text/plain", "404: Not Found");
        }
        });

    webServer.begin();
}

bool Remote::HandleFileRead(String path) {
    if (path.endsWith("/")) {
        path += "index.htm";
    }
    String contentType = "text/html";
    if (SPIFFS.exists(path)) {
        File file = SPIFFS.open(path, "r");
        size_t sent = webServer.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;
}

void Remote::HandleData(int8_t c) {
    switch (c) {
    case '1':
        modekeeper.SetMode(Modekeeper::Mode::Clock);
        break;
    case '2':
        modekeeper.SetMode(Modekeeper::Mode::SingleColor);
        break;
    case '3':
        modekeeper.SetMode(Modekeeper::Mode::Noise);
        break;
    }
}

void Remote::OnMessage(WebsocketsClient& client, WebsocketsMessage& message) {
    HandleData(message.rawData()[0]);
    client.send("Echo: " + message.data());
}

void Remote::PollAllWsClients() {
    for (auto& client : websocketClients) {
        client.poll();
    }
}

void Remote::Handle() {
    // Websocket
    if (websocketServer.poll()) {
        auto ws_client = websocketServer.accept();
        ws_client.onMessage([this](WebsocketsClient& client, WebsocketsMessage message) { OnMessage(client, message); });
        websocketClients.push_back(ws_client);
    }

    PollAllWsClients();

    webServer.handleClient();
}

