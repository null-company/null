#pragma once

#include <queue>

#include "SFML/Network.hpp"
#include "NetClientCollector.h"

/**
 * GameServer is responsible for sending state messages,
 * receiving (todo what kind of) messages, while running a
 * game simulation
 */
class GameServer : public NetClientCollector {
private:
    std::mutex m;
    std::vector<uint8_t> clientIDs;
    static uint8_t globalGameID;
    std::map<uint64_t, std::queue<net::GameMessage::ClientCommand>> entityIdToMessageQueue;
public:
    explicit GameServer(std::function<void(void)>& simulation);

    int clientCount();

    std::queue<net::GameMessage::ClientCommand>& subscribe(uint64_t entityId);

    void handleNetMessage(int clientIdx, const net::NetMessage &message) override;

    /**
     * Used for controlling clients
     * @param message
     */
    void broadcastMessage(const net::GameMessage& message);
    void broadcastMessage(const net::GameMessage::SubscriberState& message);

    void distributeMessageToSubscribers(const net::GameMessage::ClientCommand& subscriberState);

    void handleGameMessage(int clientIdx, const net::GameMessage &message);

    uint8_t getGameID(int clientIdx);

    void acceptNewClient() override;

    void disconnectClient(int idx) override;
};
