//
// Created by artemonchelus on 22.11.2021.
//

#ifndef NULL_GAME_SERVERARBITER_H
#define NULL_GAME_SERVERARBITER_H

#include <SFML/Network.hpp>
#include <list>
#include "GameServer.h"
#include "serverConfig.pb.h"
#include "NetClientCollector.h"

class ServerArbiter : public NetClientCollector {
    ServerArbiter(std::function<void()> simulationThread);

    std::vector<std::unique_ptr<GameServer>> gameServers;
    std::list<uint16_t> freePorts;
    std::map<std::string, int> roomCodeToServerNum;
public:
    ServerArbiter();

    std::string createNewGameSimulation();

    void sendGameServerConfig(sf::TcpSocket &client, const std::string &roomCode);

    void handleNetMessage(int clientIdx, const net::NetMessage &message) override;

    void sendRoomCode(sf::TcpSocket &socket, const std::string &roomCode);
};

#endif //NULL_GAME_SERVERARBITER_H
