//
// Created by artemonchelus on 02.11.2021.
//

#ifndef NULL_GAME_CLIENT_H
#define NULL_GAME_CLIENT_H

#include "SFML/Network.hpp"
#include "serverConfig.pb.h"

class Client {
private:
    char client_idx;
    sf::IpAddress gameServerAddress;
    sf::TcpSocket serverStateSocket;
    uint16_t serverStatePort;
    sf::TcpSocket clientCommandSocket;
    uint16_t clientCommandPort;

    sf::IpAddress arbiterServerAddress;
    sf::TcpSocket arbiterSocket;
    uint16_t arbiterPort;

    std::string roomCode;
private:
    void sendGenerateRoomMessage();

    void receiveServerGameRoomInfo();

public:

    Client(sf::IpAddress serverAddress, uint16_t arbiterPort);

    void createRoom();

    void connectRoom();

    void sendChatMessage(const std::string &chatMessage);

    void receiveWorldStateMessage();

    void handleChatMessage(net::ChatMessage *pMessage);
};

#endif //NULL_GAME_CLIENT_H
