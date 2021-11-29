//
// Created by artemonchelus on 02.11.2021.
//

#ifndef NULL_GAME_GAMESERVER_H
#define NULL_GAME_GAMESERVER_H

#include "SFML/Network.hpp"
#include "NetClientCollector.h"

class GameServer : public NetClientCollector {
public:
    GameServer(sf::IpAddress ipAddress, uint16_t port);

    void handleNetMessage(sf::TcpSocket &client, const net::NetMessage &message) override;

    void broadcastChatMessage(const std::string &basicString);

    void broadcastMessage(const net::NetMessage &message);
};


#endif //NULL_GAME_GAMESERVER_H
