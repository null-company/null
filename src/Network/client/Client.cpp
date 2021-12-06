#include <utils/NetMessageTransmitting.h>
#include "client/Client.h"
#include "SFML/Network.hpp"
#include "iostream"
#include "exception"
#include "plog/Log.h"
#include "serialized/serverConfig.pb.h"

Client::Client(sf::IpAddress arbiterServerAddress, uint16_t arbiterPort) {

    LOGD << "Try to connect to arbitrary server " << arbiterServerAddress << " " << arbiterPort;
    sf::Socket::Status status = arbiterSocket.connect(arbiterServerAddress, arbiterPort);
    if (status != sf::Socket::Done) {
        throw std::invalid_argument("Socket connection failed");
    }

    LOGD << "Connection to arbiter server was initiated successfully";
}

void Client::sendGenerateRoomMessage() {
    net::NetMessage message;
    message.mutable_generate_room_request();
    net::NetMessage message2;
    sendNetMessage(arbiterSocket, message);
    LOGD << "Message was send" << message.SerializePartialAsString();
}

void Client::handleRoomCodeMessage(const net::ConnectRoom &room) {
    LOGD << "try handle server game room code info";
    setRoomCode(room.room_code());
    LOGD << "Room code was received: " << roomCode;
}

void Client::connectRoom(sf::IpAddress serverAddress, uint16_t port) {
    if (gameServerSocket.connect(serverAddress, port) != sf::Socket::Done) {
        LOGE << "Connect server state socket error occurred";
        throw std::invalid_argument("Connect server state socket error occurred");
    }
}

void Client::createRoom() {
    sendGenerateRoomMessage();
    net::NetMessage message = receiveNetMessage(arbiterSocket);
    if (!message.has_connect_room()) {
        throw std::invalid_argument("Expected room code");
    }
    connectRoom(message.connect_room().room_code());
}

void Client::connectRoom(const std::string &roomCode) {
    setRoomCode(roomCode);
    askGameServerConfigByRoomCode();
    net::NetMessage message = receiveNetMessage(arbiterSocket);
    if (!message.has_server_config()) {
        throw std::invalid_argument("Expected room code");
    }
    handleServerConfigMessageAndConnect(message.server_config());
}

void Client::sendChatMessage(const std::string &chatMessage) {
    net::GameMessage message;
    std::string *message_field = message.mutable_chat_message()->mutable_message();
    *(message_field) = chatMessage;
    sendNetMessage(gameServerSocket, message);
    LOGD << "Chat message was send\n" << chatMessage << std::endl;
}

void Client::handleChatMessage(const net::ChatMessage &chatMessage) {
    std::cout << chatMessage.message();
}

void Client::askGameServerConfigByRoomCode() {
    LOGD << "Ask arbiter server for GameServerConfig";
    net::NetMessage message;
    *(message.mutable_connect_room()->mutable_room_code()) = roomCode;
    sendNetMessage(arbiterSocket, message);
}

void Client::handleGameServerMessage(const net::GameMessage &message) {
    switch (message.message_case()) {
        case net::GameMessage::kChatMessage:
            handleChatMessage(message.chat_message());
            break;
        default:
            LOGE << "Unexpected message type from world state socket";
            throw std::invalid_argument("Unexpected message type from world state socket");
            break;
    }
}

void Client::handleServerConfigMessageAndConnect(const net::GameServerConfig &config) {
    sf::IpAddress gameServerAddress(config.v4());
    uint16_t gameServerPort = config.server_port();
    setRoomCode(config.room_code());

    LOGD << "Game server address " << gameServerAddress;
    LOGD << "Game server port " << gameServerPort;
    LOGD << "Room code is" << roomCode;
    LOGD << "Try to connect";
    connectRoom(gameServerAddress, gameServerPort);
}

void Client::handleChatMessage(net::ChatMessage *pMessage) {
    std::cout << pMessage->message() << std::endl;
}

void Client::setRoomCode(const std::string &roomCode) {
    this->roomCode = roomCode;
}

std::string Client::getRoom() {
    return roomCode;
}

void Client::handleServerArbiterMessage(const net::NetMessage &message) {

}

sf::TcpSocket &Client::getGameServerSocket() {
    return gameServerSocket;
}

