#include "Client.h"
#include "SFML/Network.hpp"
#include "iostream"
#include "exception"
#include "plog/Log.h"
#include "serialized/serverConfig.pb.h"

Client::Client(sf::IpAddress serverAddress, uint16_t arbiterPort) :
        arbiterServerAddress(serverAddress),
        arbiterPort(arbiterPort) {
    LOG(plog::debug) << "Try to connect to arbitrary server";
    sf::Socket::Status status = arbiterSocket.connect(arbiterServerAddress, arbiterPort);
    if (status != sf::Socket::Done) {
        throw std::invalid_argument("Socket connection failed");
    }

    LOG(plog::debug) << "Connection to arbiter server was initiated successfully";
}

void Client::sendGenerateRoomMessage() {
    net::NetMessage message;
    message.mutable_generate_room_request();
    sendNetMessage(arbiterSocket, message);
    LOGD << "Message was send" << message.SerializePartialAsString();
}

void Client::receiveServerGameRoomInfo() {
    LOGD << "try receive server game room info";
    net::NetMessage message = receiveNetMessage(serverStateSocket);
    if (message.has_server_config()) {
        LOGE << "Expected game server info";
        throw std::invalid_argument("Expected game server info");
    }
    const net::GameServerConfig &config = message.server_config();

    gameServerAddress = sf::IpAddress(config.v4());
    clientCommandPort = config.server_command_port();
    serverStatePort = config.server_state_port();
    roomCode = config.room_code();
    LOGD << "Game server address " << gameServerAddress;
    LOGD << "Game server state port " << serverStatePort;
    LOGD << "Game server command port" << clientCommandPort;
    LOGD << "Room code is " << roomCode;
}

void Client::connectRoom() {
    if (serverStateSocket.connect(gameServerAddress, serverStatePort) != sf::Socket::Done) {
        LOGE << "Connect server state socket error occured";
        throw std::invalid_argument("Connect server state socket error occured");
    }
    if (clientCommandSocket.connect(gameServerAddress, clientCommandPort) != sf::Socket::Done) {
        LOGE << "Connect server command socket error occurred";
        throw std::invalid_argument("Connect server command socket error occured");
    }
}

void Client::createRoom() {
    sendGenerateRoomMessage();
    receiveServerGameRoomInfo();
    connectRoom();
}

void Client::sendChatMessage(const std::string &chatMessage) {
    net::NetMessage message;
    std::string *message_field = message.mutable_chat_message()->mutable_message();
    *(message_field) = chatMessage;
    sendNetMessage(clientCommandSocket, message);
    LOGD << "Chat message was send\n" << chatMessage << std::endl;
}

void Client::handleChatMessage(net::ChatMessage *chatMessage) {
    std::cout << chatMessage->message();
}

void Client::receiveWorldStateMessage() {
    net::NetMessage message = receiveNetMessage(serverStateSocket);
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

