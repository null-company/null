#pragma once
#include "SFML/Network.hpp"
#include "serverConfig.pb.h"

class Client {
private:
    char client_idx;
    sf::TcpSocket gameServerSocket;
    sf::TcpSocket arbiterSocket;
    std::string roomCode;
private:
    void sendGenerateRoomMessage();

    void askGameServerConfigByRoomCode();

    void setRoomCode(const std::string &newRoomCode);

public:

    Client(sf::IpAddress serverAddress, uint16_t arbiterPort);
    ~Client();
    std::string getRoom();

    sf::TcpSocket& getGameServerSocket();
    void createRoom();

    void connectRoom(sf::IpAddress serverAddress, uint16_t port);

    void sendChatMessage(const std::string &chatMessage);

    void handleGameServerMessage(const net::GameMessage &message);

    void handleChatMessage(net::ChatMessage *pMessage);

    void handleRoomCodeMessage(const net::ConnectRoom &room);

    void handleServerArbiterMessage(const net::NetMessage &message);

    void handleServerConfigMessageAndConnect(const net::GameServerConfig &config);

    static void handleChatMessage(const net::ChatMessage &chatMessage);

    void connectRoom(const std::string &roomCode);
};
