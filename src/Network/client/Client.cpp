#include <utils/NetMessageTransmitting.h>
#include "client/Client.h"
#include "SFML/Network.hpp"
#include "iostream"
#include "exception"
#include "plog/Log.h"
#include "serialized/serverConfig.pb.h"

Client::Client(sf::IpAddress serverAddress, uint16_t arbiterPort) :
        arbiterServerAddress(serverAddress),
        arbiterPort(arbiterPort) {
    LOGD << "Try to connect to arbitrary server";
    sf::Socket::Status status = arbiterSocket.connect(arbiterServerAddress, arbiterPort);
    if (status != sf::Socket::Done) {
        throw std::invalid_argument("Socket connection failed");
    }

    LOGD << "Connection to arbiter server was initiated successfully";
}

void Client::sendGenerateRoomMessage() {
    net::NetMessage message;
    message.mutable_generate_room_request();
    sendNetMessage(arbiterSocket, message);
    LOGD << "Message was send" << message.SerializePartialAsString();
}

void Client::handleRoomCodeMessage(const net::ConnectRoom &room) {
    LOGD << "try handle server game room code info";
    roomCode = room.room_code();
    LOGD << "Room code was received: " << roomCode;
}

void Client::connectRoom() {
    if (gameServerSocket.connect(gameServerAddress, gameServerPort) != sf::Socket::Done) {
        LOGE << "Connect server state socket error occurred";
        throw std::invalid_argument("Connect server state socket error occurred");
    }
}

void Client::createRoom() {
    sendGenerateRoomMessage();
}

void Client::sendChatMessage(const std::string &chatMessage) {
    net::NetMessage message;
    std::string *message_field = message.mutable_chat_message()->mutable_message();
    *(message_field) = chatMessage;
    sendNetMessage(gameServerSocket, message);
    LOGD << "Chat message was send\n" << chatMessage << std::endl;
}

void Client::handleChatMessage(const net::ChatMessage &chatMessage) {
    std::cout << chatMessage.message();
}

void Client::handleServerArbiterMessage(const net::NetMessage &message) {
    if (message.has_connect_room()) {
        LOGD << "Room code message receive";
        handleRoomCodeMessage(message.connect_room());
        askGameServerConfigByRoomCode();
    } else if (message.has_server_config()) {
        handleServerConfigMessage(message.server_config());
        connectRoom();
    } else if (message.has_chat_message()) {
        handleChatMessage(message.chat_message());
    } else {
        LOGE << "Unexpected message type was received";
    }
}

void Client::askGameServerConfigByRoomCode() {
    LOGD << "Ask arbiter server for GameServerConfig";
    net::NetMessage message;
    *(message.mutable_connect_room()->mutable_room_code()) = roomCode;
    sendNetMessage(arbiterSocket, message);
}

void Client::receiveWorldStateMessage() {
    net::NetMessage message = receiveNetMessage(gameServerSocket);
    switch (message.body_case()) {
        case net::NetMessage::kChatMessage:
            handleChatMessage(message.mutable_chat_message());
            break;
        default:
            LOGE << "Unexpected message type from world state socket";
            throw std::invalid_argument("Unexpected message type from world state socket");
            break;
    }
}

void Client::handleServerConfigMessage(const net::GameServerConfig &config) {
    gameServerAddress = sf::IpAddress(config.v4());
    gameServerPort = config.server_port();
    roomCode = config.room_code();

    LOGD << "Game server address " << gameServerAddress;
    LOGD << "Game server port " << gameServerPort;
    LOGD << "Room code is " << roomCode;
}

