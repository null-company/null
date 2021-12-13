#include "utils/NetMessageTransmitting.h"
#include "plog/Log.h"
#include <exceptions/NetworkException.h>


net::NetMessage receiveNetMessage(sf::TcpSocket &socket) {
    LOGD << "Try to receive message";
    net::NetMessage message;
    sf::Packet packet;
    auto status = socket.receive(packet);
    if (status != sf::Socket::Done) {
        LOGD << "Cannot receive message";
        throw ClientReceiveException("Error occurred while trying to receive some message from client", status);
    }
    std::string resultString;
    packet >> resultString;
    message.ParseFromString(resultString);
    LOGD << "Message was received successfully: " << resultString;
    return message;
}


void sendNetMessage(sf::TcpSocket &socket, const net::NetMessage &message) {
    sf::Packet packet;
    packet << message.SerializeAsString();
    auto status = socket.send(packet);
    if (status != sf::Socket::Done) {
        LOGD << "Cannot send message";
        throw ClientReceiveException("Error occurred while trying to send message to client", status);
    }
    LOGD << "Message send successful";
}

void sendGameMessage(sf::TcpSocket &socket, const net::GameMessage &message) {
    net::NetMessage message1;
    *message1.mutable_game_message() = message;
    sendNetMessage(socket, message1);
}

