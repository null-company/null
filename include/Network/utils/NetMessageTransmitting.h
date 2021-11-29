#ifndef NULL_GAME_NETMESSAGETRANSMITTING_H
#define NULL_GAME_NETMESSAGETRANSMITTING_H

#include <serverConfig.pb.h>
#include <SFML/Network.hpp>

net::NetMessage receiveNetMessage(sf::TcpSocket &socket);

void sendNetMessage(sf::TcpSocket &socket, const net::NetMessage &message);

#endif
