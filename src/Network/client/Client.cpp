#include <utils/NetMessageTransmitting.h>
#include "client/Client.h"
#include "SFML/Network.hpp"
#include "iostream"
#include "plog/Log.h"
#include "serialized/serverConfig.pb.h"
#include "exceptions/NetworkException.h"

Client::Client(sf::IpAddress arbiterServerAddress, uint16_t arbiterPort) {
    LOGD << "Try to connect to arbiter server " << arbiterServerAddress << " " << arbiterPort;
    sf::Socket::Status status = arbiterSocket.connect(arbiterServerAddress, arbiterPort);
    gameServerSocket.setBlocking(false);
    if (status != sf::Socket::Done) {
        throw NetworkException("Socket connection failed");
    }
    LOGD << "Connection to arbiter server was initiated successfully";
}

void Client::sendGenerateRoomMessage() {
    net::NetMessage message;
    message.mutable_generate_room_request();
    net::NetMessage message2;
    sendNetMessage(arbiterSocket, message);
    LOGD << "Message was send: " << message.ShortDebugString();
}

void Client::handleRoomCodeMessage(const net::ConnectRoom &room) {
    LOGD << "try handle server game room code info";
    setRoomCode(room.room_code());
    LOGD << "Room code was received: " << roomCode;
}

void Client::connectRoom(sf::IpAddress serverAddress, uint16_t port) {
    gameServerSocket.setBlocking(true);
    auto status = gameServerSocket.connect(serverAddress, port);
    if (status != sf::Socket::Done) {
        throw ReceiveException("Game server connection failed", status);
    }
}

void Client::createRoom() {
    LOGD << "Creating room";
    arbiterSocket.setBlocking(true);
    sendGenerateRoomMessage();
    LOGD << "Sent generate room message";
    net::NetMessage message = receiveNetMessage(arbiterSocket);
    if (!message.has_connect_room()) {
        throw std::invalid_argument("Expected room code");
    }
    connectRoom(message.connect_room().room_code());
    LOGD << "Connected to room";
    arbiterSocket.setBlocking(false);
}

net::GameMessage Client::receiveGameMessage() {
    net::NetMessage message = receiveNetMessage(this->getGameServerSocket());
    if (!message.has_game_message()) {
        throw NetworkException("Unexpected type message");
    }
    return message.game_message();
}


void Client::connectRoom(const std::string &roomCode) {
    gameServerSocket.setBlocking(true);
    setRoomCode(roomCode);
    askGameServerConfigByRoomCode();
    net::NetMessage message = receiveNetMessage(arbiterSocket);
    if (!message.has_server_config()) {
        throw std::invalid_argument("Expected server config");
    }
    handleServerConfigMessageAndConnect(message.server_config());
    gameServerSocket.setBlocking(false);
}

//void Client::sendChatMessage(const std::string &chatMessage) {
//    net::GameMessage message;
//    std::string *message_field = message.mutable_chat_message()->mutable_message();
//    *(message_field) = chatMessage;
//    sendGameMessage(gameServerSocket, message);
//    LOGD << "Chat message was send\n" << chatMessage;
//}

void Client::askGameServerConfigByRoomCode() {
    LOGD << "Ask arbiter server for GameServerConfig";
    net::NetMessage message;
    *(message.mutable_connect_room()->mutable_room_code()) = roomCode;
    sendNetMessage(arbiterSocket, message);
}

void Client::handleServerConfigMessageAndConnect(const net::GameServerConfig &config) {
    sf::IpAddress gameServerAddress(config.v4());
    uint16_t gameServerPort = config.server_port();
    setRoomCode(config.room_code());

    LOGD << "Game server address: " << gameServerAddress;
    LOGD << "Game server port: " << gameServerPort;
    LOGD << "Room code is: " << roomCode;
    LOGD << "Try to connect...";
    connectRoom(gameServerAddress, gameServerPort);
}

void Client::setRoomCode(const std::string &newRoomCode) {
    roomCode = newRoomCode;
}

std::string Client::getRoomCode() {
    return roomCode;
}

sf::TcpSocket &Client::getGameServerSocket() {
    return gameServerSocket;
}

Client::~Client() {
    gameServerSocket.disconnect();
    arbiterSocket.disconnect();
}

