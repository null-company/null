#pragma once

#include "serialized/serverConfig.pb.h"
#include <SFML/Network.hpp>

namespace null::Network::Utils {
    net::NetMessage receiveNetMessage(sf::TcpSocket &socket);

    void sendNetMessage(sf::TcpSocket &socket, const net::NetMessage &message);

    void sendGameMessage(sf::TcpSocket &socket, const net::GameMessage &message);
}
