//
// Created by artemonchelus on 02.11.2021.
//

#ifndef NULL_GAME_SERVER_H
#define NULL_GAME_SERVER_H

#include "SFML/Network.hpp"
#include "ClientCollector.h"

class Server : public ClientCollector {
public:
    Server(sf::IpAddress ipAddress, uint16_t port);
};


#endif //NULL_GAME_SERVER_H
