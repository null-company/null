//
// Created by artemonchelus on 02.11.2021.
//

#ifndef NULL_GAME_CHANNEL_H
#define NULL_GAME_CHANNEL_H
#include "SFML/Network.hpp"

class Channel {
    sf::TcpSocket socket;
    void sendMessage(Message message);
    Channel(sf::TcpSocket&& socket): socket(std::move(socket)){}
};



#endif //NULL_GAME_CHANNEL_H
