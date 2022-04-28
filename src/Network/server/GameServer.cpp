#include <plog/Log.h>
#include "server/GameServer.h"
#include "utils/NetMessageTransmitting.h"

GameServer::GameServer() :
        NetClientCollector() {
}

uint8_t GameServer::globalGameID = 1;

void GameServer::broadcastMessage(const net::GameMessage &message, int clientId) {
    for (int i = 0; i < clientCount(); i++) {
        if (i == clientId)
            continue;
        sendGameMessage(getClient(i), message);
    }
}

void GameServer::handleNetMessage(int clientIdx, const net::NetMessage &message) {
    if (!message.has_game_message()) {
        throw std::invalid_argument("This net message must contain game message");
    }
    handleGameMessage(clientIdx, message.game_message());
}


// This is the main function must be used
void GameServer::handleGameMessage(int clientIdx, const net::GameMessage &message) {
    auto idMsg = message;
    idMsg.set_game_id(getGameID(clientIdx));
    broadcastMessage(idMsg, clientIdx);
}

void GameServer::acceptNewClient() {
    NetClientCollector::acceptNewClient();
    clientIDs.emplace_back(globalGameID++);
}

void GameServer::disconnectClient(int idx) {
    NetClientCollector::disconnectClient(idx);
    if (idx != clientIDs.size() - 1) {
        std::swap(clientIDs[idx], clientIDs.back());
    }
    clientIDs.resize(clientIDs.size() - 1);
}

int GameServer::clientCount() {
    assert(clients.size() == clientIDs.size());
    return clients.size();
}

uint8_t GameServer::getGameID(int clientIdx) {
    return clientIDs[clientIdx];
}
