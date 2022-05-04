#pragma once

#include <queue>
#include <set>

#include "SFML/Network/IpAddress.hpp"
#include "plog/Log.h"
#include "SFML/System/Thread.hpp"
#include "client/Client.h"
#include "utils/NetMessageTransmitting.h"
#include "serialized/serverConfig.pb.h"

class ClientNetworkManager {
private:
    Client client;
    std::map<uint64_t, std::queue<net::GameMessage::SubscriberState>> channels;
//    std::map<uint64_t, std::set<uint64_t>> messageIdToSubscribedPlayersIds;

    void ensurePlayerChannel(uint64_t subscriberEntityId);

//    void ensureMessageIdSet(uint64_t subscriberEntityId);

public:

    Client& getClient();
    ClientNetworkManager(sf::IpAddress address, uint16_t port);

    std::queue<net::GameMessage::SubscriberState>& subscribe(uint64_t entityId);

    void unsubscribe(uint64_t entityId);

    net::GameMessage receiveMessage();

    /**
     * Distribute a message to subscribers
     * @param subscriberState message to distribute
     */
    void distributeMessageToSubscribers(net::GameMessage::SubscriberState& subscriberState);

    std::queue<net::GameMessage::SubscriberState>& getOrCreateSubscriberQueue(uint64_t playerId);

//    std::set<uint64_t>& getSubscribedSet(uint64_t subscriberEntityId);

};
