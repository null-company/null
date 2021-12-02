#include <plog/Log.h>
#include "server/GameServer.h"
#include "utils/NetMessageTransmitting.h"

GameServer::GameServer(sf::IpAddress ipAddress, uint16_t port) :
        NetClientCollector(ipAddress, port) {

    LOGD << "Game server was initiated successfully: " << ipAddress << " " << port;
}

void GameServer::handleNetMessage(sf::TcpSocket &client, const net::NetMessage &message) {
    if (message.has_chat_message()) {
        broadcastMessage(message);
    }
}

void GameServer::broadcastMessage(const net::NetMessage &message) {
    for (auto &client: clients) {
        sendNetMessage(*client, message);
    }
}

