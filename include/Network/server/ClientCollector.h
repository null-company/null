#ifndef NULL_GAME_CLIENTCOLLECTOR_H
#define NULL_GAME_CLIENTCOLLECTOR_H

#include <SFML/Network.hpp>
#include <cstdint>
#include <list>
#include <serverConfig.pb.h>

class ClientCollector {
public:
    sf::TcpListener listener;
    std::vector<sf::TcpSocket> clients;
    std::list<ssize_t> freeClientSlots;

    ClientCollector(sf::IpAddress ipAddress, uint16_t port);

    void acceptNewClient();
};

#endif //NULL_GAME_CLIENTCOLLECTOR_H
