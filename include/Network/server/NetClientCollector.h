#ifndef NULL_GAME_NETCLIENTCOLLECTOR_H
#define NULL_GAME_NETCLIENTCOLLECTOR_H

#include <SFML/Network.hpp>
#include <cstdint>
#include <list>
#include <serverConfig.pb.h>

class NetClientCollector {
private:
    sf::Thread simulationThread;
public:
    sf::TcpListener listener;
    std::vector<std::unique_ptr<sf::TcpSocket>> clients;
    std::list<ssize_t> freeClientSlots;

    sf::IpAddress ipAddress;
    uint16_t port;

    NetClientCollector(sf::IpAddress ipAddress, uint16_t port);

    sf::TcpSocket &getFirstReadySocket();

    void acceptNewClient();

    virtual void handleNetMessage(sf::TcpSocket &client, const net::NetMessage &message) = 0;

    void launch();
};

#endif //NULL_GAME_NETCLIENTCOLLECTOR_H
