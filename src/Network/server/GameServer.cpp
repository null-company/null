#include <plog/Log.h>
#include "server/GameServer.h"
#include "utils/NetMessageTransmitting.h"

GameServer::GameServer(sf::IpAddress ipAddress, uint16_t port) :
        NetClientCollector(ipAddress, port) {
    LOGD << "Game server was initiated successfully: " << ipAddress << " " << port;
}

void GameServer::broadcastMessage(const net::GameMessage &message) {
    for (auto &client: clients) {
        sendNetMessage(*client, message);
    }
}

void GameServer::broadcastChatMessage(const std::string &basicString) {
    net::GameMessage message;
    *(message.mutable_chat_message()->mutable_message()) = basicString;
    broadcastMessage(message);
}

void GameServer::handleNetMessage(sf::TcpSocket &client, const net::NetMessage &message) {
    if (!message.has_game_message()) {
        throw std::invalid_argument("This net message must contain game message");
    }
    handleGameMessage(client, message.game_message());
}


// This is the main function must be used
void GameServer::handleGameMessage(sf::TcpSocket &client, const net::GameMessage &message) {
    if (message.has_chat_message()) {
        broadcastMessage(message);
    }
}

