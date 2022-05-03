#pragma once

#include <SFML/Network.hpp>
#include <list>

#include "GameServer.h"
#include "serialized/serverConfig.pb.h"
#include "NetClientCollector.h"

/**
 * ServerArbiter is responsible for starting new GameServer's and
 * creating rooms.
 */
class ServerArbiter : public NetClientCollector {
private:

    /**
     * GameServers that this arbiter is running
     * Todo add support for actually running multiple of those
     */
    std::vector<std::unique_ptr<GameServer>> gameServers;
    std::list<uint16_t> freePorts;
    std::map<std::string, int> roomCodeToServerNum;
public:
    ServerArbiter();

    /**
     * Serves as a server arbiter that also manages network
     * The simulation starts when players are connected
     * @param simulation a function to start in a different thread which will perform simulation
     */
    ServerArbiter(std::function<void()> simulation);

    std::string createNewGameSimulation();

    void sendGameServerConfig(sf::TcpSocket &client, const std::string &roomCode);

    void handleNetMessage(int clientIdx, const net::NetMessage &message) override;

    void sendRoomCode(sf::TcpSocket &socket, const std::string &roomCode);
    ~ServerArbiter() override;
};
