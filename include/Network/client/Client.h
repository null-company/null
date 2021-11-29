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
    sf::TcpSocket gameServerSocket;
    sf::IpAddress gameServerAddress;
    uint16_t gameServerPort;

    sf::IpAddress arbiterServerAddress;
    sf::TcpSocket arbiterSocket;
    uint16_t arbiterPort;

    std::string roomCode;
private:
    void sendGenerateRoomMessage();

    void askGameServerConfigByRoomCode();

public:

    Client(sf::IpAddress serverAddress, uint16_t arbiterPort);

    void createRoom();

    void connectRoom();

    void sendChatMessage(const std::string &chatMessage);

    void receiveWorldStateMessage();

    void handleChatMessage(net::ChatMessage *pMessage);

    void handleRoomCodeMessage(const net::ConnectRoom &room);

    void handleServerArbiterMessage(const net::NetMessage &message);

    void handleServerConfigMessage(const net::GameServerConfig &config);

    static void handleChatMessage(const net::ChatMessage &chatMessage);
};

#endif //NULL_GAME_CLIENT_H
