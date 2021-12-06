#include "utils/NetMessageTransmitting.h"
#include "plog/Log.h"

net::NetMessage receiveNetMessage(sf::TcpSocket &socket) {
    LOGD << "Receive some some net message";
    net::NetMessage message;
    sf::Packet packet;
    if (socket.receive(packet) != sf::Socket::Done) {
        LOGE << "Error occurred while trying to receive some message from";
        throw std::invalid_argument("Error occurred while trying to receive some message from");
    }
    std::string resultString;
    packet >> resultString;
    message.ParseFromString(resultString);
    return message;
}


void sendNetMessage(sf::TcpSocket &socket, const net::NetMessage &message) {
    LOGD << "Sending Some Message";
    sf::Packet packet;
    packet << message.SerializeAsString();
    if (socket.send(packet) != sf::Socket::Done) {
        LOGE << "Error occurred while sending message";
        throw std::invalid_argument("Error occurred while sending message");
    }
}

void sendNetMessage(sf::TcpSocket &socket, const net::GameMessage &message) {
    net::NetMessage message1;
    *message1.mutable_game_message() = message;
    sendNetMessage(socket, message1);
}

