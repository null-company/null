#pragma once

#include "SFML/Network/IpAddress.hpp"
#include "plog/Log.h"
#include "SFML/System/Thread.hpp"
#include "client/Client.h"
#include "utils/NetMessageTransmitting.h"
#include <queue>
#include <set>


class ClientNetworkManager {
private:
    Client client;
    std::map<int, std::queue<net::GameMessage>> channels;
    std::map<int, std::set<int>> messageIdToSubscribedPlayersIds;

    void ensurePlayerChannel(int playerId);

    void ensureMessageIdSet(int messageId);

public:

    Client & getClient();
    ClientNetworkManager(sf::IpAddress address, uint16_t port);

    std::queue<net::GameMessage> &subscribe(int playerId, int messageId);

    void unsubscribe(int playerId, int messageId);

    net::GameMessage receiveMessage();

    void multiplexMessage(net::GameMessage &gameMessage);

    std::queue<net::GameMessage> &getQueue(int playerId);

    std::set<int> &getSubscribedSet(int messageId);

};
