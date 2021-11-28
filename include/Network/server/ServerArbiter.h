//
// Created by artemonchelus on 22.11.2021.
//

#ifndef NULL_GAME_SERVERARBITER_H
#define NULL_GAME_SERVERARBITER_H

#include <SFML/Network.hpp>
#include <list>
#include "Server.h"
#include "serverConfig.pb.h"
#include "ClientCollector.h"

class ServerArbiter : public ClientCollector {
    std::vector<Server> gameServers;
    std::list<uint16_t> freePorts;
public:
    ServerArbiter(sf::IpAddress ipAddress, uint16_t port);
    net::GameServerConfig createNewGameSimulation();
    void sendServerGameRoomInfo(sf::TcpSocket &client);

};

#endif //NULL_GAME_SERVERARBITER_H
