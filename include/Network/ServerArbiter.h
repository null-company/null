//
// Created by artemonchelus on 22.11.2021.
//

#ifndef NULL_GAME_SERVERARBITER_H
#define NULL_GAME_SERVERARBITER_H

#include <SFML/Network.hpp>

class ServerArbiter {
    sf::TcpListener listener;
public:
    ServerArbiter(sf::IpAddress ipAddress, uint16_t port);
};

#endif //NULL_GAME_SERVERARBITER_H
