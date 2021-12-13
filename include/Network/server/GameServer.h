//
// Created by artemonchelus on 02.11.2021.
//

#ifndef NULL_GAME_GAMESERVER_H
#define NULL_GAME_GAMESERVER_H

#include "SFML/Network.hpp"
#include "NetClientCollector.h"

class GameServer : public NetClientCollector {
public:
    GameServer();

    void handleNetMessage(sf::TcpSocket &client, const net::NetMessage &message) override;

    void broadcastMessage(const net::GameMessage &message);

    void handleGameMessage(sf::TcpSocket &client, const net::GameMessage &message);

};


#endif //NULL_GAME_GAMESERVER_H
