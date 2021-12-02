//
// Created by artemonchelus on 28.11.2021.
//

#include <server/NetClientCollector.h>
#include <SFML/Network.hpp>
#include <cstdint>
#include <plog/Log.h>
#include <utils/NetMessageTransmitting.h>

NetClientCollector::NetClientCollector(sf::IpAddress ipAddress, uint16_t port) :
        ipAddress(ipAddress),
        port(port),
        listener(),
        simulationThread([this]() {
            bool isActive = true;
            while (isActive) {
                sf::TcpSocket &client = this->getFirstReadySocket();
                net::NetMessage message = receiveNetMessage(client);
                this->handleNetMessage(client, message);
                if (0 != 0) {
                    break;
                }
            }
        }) {

    if (listener.listen(port, ipAddress) != sf::Socket::Done) {
        LOGE << "Error occurred while listening " << ipAddress.toString() << " " << port;
        throw std::invalid_argument("Error occurred while listening");
    }
    LOGD << "Listen was started successfully";
}

void NetClientCollector::acceptNewClient() {
    if (freeClientSlots.empty()) {
        clients.emplace_back();
        freeClientSlots.push_back(clients.size() - 1);
    }
    sf::TcpSocket &newClient = **(clients.begin() + freeClientSlots.front());
    freeClientSlots.pop_front();
    if (listener.accept(newClient) != sf::Socket::Done) {
        LOGD << "Cannot accept new client";
        throw std::invalid_argument("Cannot accept new client");
    }
    LOGD << "New client was accepted";
}


sf::TcpSocket &NetClientCollector::getFirstReadySocket() {
    // TODO not generate it every time i want to receive message
    // TODO not allow one socket to take the whole CPU time
    LOGD << "Wait for the first ready socket";
    sf::SocketSelector socketSelector;
    for (auto &client: clients) {
        socketSelector.add(*client);
    }
    socketSelector.add(listener);
    socketSelector.wait();
    if (socketSelector.isReady(listener)) {
        acceptNewClient();
        //TODO: Remove this recursion stuff
        return getFirstReadySocket();
    };
    for (auto &client: clients) {
        if (socketSelector.isReady(*client)) {
            return *client;
        };
    }
    return *clients[0];
}

void NetClientCollector::launch() {
    simulationThread.launch();
    LOGD << "New NetClientCollector was launched for new incoming connections";
}

