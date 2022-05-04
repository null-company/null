#include "client/ClientNetworkManager.h"

ClientNetworkManager::ClientNetworkManager(sf::IpAddress address, uint16_t port)
        : client(address, port) {
}

std::queue<net::GameMessage::SubscriberState>& ClientNetworkManager::subscribe(uint64_t entityId) {
    LOGD << "Object with id: " << entityId << " subscribed";

    auto& queue = getOrCreateSubscriberQueue(entityId);
    return queue;
}

net::GameMessage ClientNetworkManager::receiveMessage() {
    return client.receiveGameMessage();
}

void ClientNetworkManager::distributeMessageToSubscribers(net::GameMessage::SubscriberState& subscriberState) {
    uint64_t entityId =  subscriberState.subscriber_id();
    LOGD << "Try to distribute message for entity with id " << entityId;
//    auto& playerIdSet = getSubscribedSet(entityId);
//    for (auto playerId: playerIdSet) {
//        auto& queue = getOrCreateSubscriberQueue(playerId);
//        queue.push(subscriberState);
//    }
    auto& queue = getOrCreateSubscriberQueue(entityId);
    queue.push(subscriberState);
}

std::queue<net::GameMessage::SubscriberState>& ClientNetworkManager::getOrCreateSubscriberQueue(uint64_t playerId) {
    ensurePlayerChannel(playerId);
    return channels.at(playerId);
}

//std::set<uint64_t>& ClientNetworkManager::getSubscribedSet(uint64_t subscriberEntityId) {
//    ensurePlayerChannel(subscriberEntityId);
//    return messageIdToSubscribedPlayersIds.at(subscriberEntityId);
//}

//void ClientNetworkManager::ensureMessageIdSet(uint64_t subscriberEntityId) {
//    if (!messageIdToSubscribedPlayersIds.contains(subscriberEntityId)) {
//        LOGD << "New set with players id need to be created for this message id: " << subscriberEntityId;
//        messageIdToSubscribedPlayersIds.insert(
//                std::pair<uint64_t, std::set<uint64_t>>(subscriberEntityId, std::set<uint64_t>())
//        );
//    }
//}

void ClientNetworkManager::ensurePlayerChannel(uint64_t subscriberEntityId) {
    if (!channels.contains(subscriberEntityId)) {
        LOGD << "New channel need to be created for player id: " << subscriberEntityId;
        channels.insert({subscriberEntityId, std::queue<net::GameMessage::SubscriberState>()});
    }
}

void ClientNetworkManager::unsubscribe(uint64_t entityId) {
    ensurePlayerChannel(entityId);
    // todo
}

Client& ClientNetworkManager::getClient() {
    return client;
}
