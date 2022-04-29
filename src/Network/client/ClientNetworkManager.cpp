#include "client/ClientNetworkManager.h"

ClientNetworkManager::ClientNetworkManager(sf::IpAddress address, uint16_t port)
        : client(address, port) {
}

std::queue<net::GameMessage>& ClientNetworkManager::subscribe(int playerId, int messageId) {
    LOGD << "Player with id: " << playerId << " subscribed on message with id: " << messageId;

    auto& queue = getQueue(playerId);
    auto& subscribedSet = getSubscribedSet(messageId);
    subscribedSet.insert(playerId);
    return queue;
}

net::GameMessage ClientNetworkManager::receiveMessage() {
    return client.receiveGameMessage();
}

void ClientNetworkManager::multiplexMessage(net::GameMessage& gameMessage) {
    int messageType = gameMessage.message_case();
    LOGD << "Try to multiplex message with message type: " << messageType;
    auto& playerIdSet = getSubscribedSet(messageType);
    for (auto playerId: playerIdSet) {
        auto& queue = getQueue(playerId);
        queue.push(gameMessage);
    }
}

std::queue<net::GameMessage>& ClientNetworkManager::getQueue(int playerId) {
    ensurePlayerChannel(playerId);
    return channels.at(playerId);
}

std::set<int>& ClientNetworkManager::getSubscribedSet(int messageId) {
    ensureMessageIdSet(messageId);
    return messageIdToSubscribedPlayersIds.at(messageId);
}

void ClientNetworkManager::ensureMessageIdSet(int messageId) {
    if (!messageIdToSubscribedPlayersIds.contains(messageId)) {
        LOGD << "New set with players id need to be created for this message id: " << messageId;
        messageIdToSubscribedPlayersIds.insert(std::pair<int, std::set<int>>(messageId, std::set<int>()));
    }
}

void ClientNetworkManager::ensurePlayerChannel(int playerId) {
    if (!channels.contains(playerId)) {
        LOGD << "New channel need to be created for player id: " << playerId;
        channels.insert(std::pair<int, std::queue<net::GameMessage>>(playerId, std::queue<net::GameMessage>()));
    }
}

void ClientNetworkManager::unsubscribe(int playerId, int messageId) {
    ensurePlayerChannel(playerId);
    ensureMessageIdSet(messageId);
    if (messageIdToSubscribedPlayersIds.at(messageId).contains(playerId)) {
        messageIdToSubscribedPlayersIds.at(messageId).erase(playerId);
    }
}

Client& ClientNetworkManager::getClient() {
    return client;
}
