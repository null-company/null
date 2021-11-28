//
// Created by artemonchelus on 28.11.2021.
//

#include <server/ClientCollector.h>
#include <SFML/Network.hpp>
#include <cstdint>
#include <plog/Log.h>

ClientCollector::ClientCollector(sf::IpAddress ipAddress, uint16_t port) {
    if (listener.listen(port, ipAddress) != sf::Socket::Done) {
        LOGD << "Listen was started successfully";
    }
}

void ClientCollector::acceptNewClient() {
    if (freeClientSlots.empty()) {
        clients.emplace_back();
        freeClientSlots.push_back(clients.size() - 1);
    }
    sf::TcpSocket &newClient = *(clients.begin() + freeClientSlots.front());
    freeClientSlots.pop_front();
    if (listener.accept(newClient) != sf::Socket::Done) {
        LOGD << "Cannot accept new client";
        throw std::invalid_argument("Cannot accept new client");
    }
    LOGD << "New client was accepted";
}


