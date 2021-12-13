//
// Created by artemonchelus on 02.11.2021.
//

#ifndef NULL_GAME_GAMESERVER_H
#define NULL_GAME_GAMESERVER_H

#include "SFML/Network.hpp"
#include "NetClientCollector.h"

class GameServer : public NetClientCollector {
private:
    std::vector<uint8_t> clientIDs;
    static uint8_t globalGameID;
public:
    GameServer();

    int clientCount();

    void handleNetMessage(int clientIdx, const net::NetMessage &message) override;

    void broadcastMessage(const net::GameMessage &message);

    void handleGameMessage(int clientIdx, const net::GameMessage &message);

    uint8_t getGameID(int clientIdx);

    void acceptNewClient() override;

    void disconnectClient(int idx) override;
};


#endif //NULL_GAME_GAMESERVER_H
