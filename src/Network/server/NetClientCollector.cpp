//
// Created by artemonchelus on 28.11.2021.
//

#include <server/NetClientCollector.h>
#include <SFML/Network.hpp>
#include <cstdint>
#include <utility>
#include <plog/Log.h>
#include <utils/NetMessageTransmitting.h>
#include "exceptions/NetworkException.h"


void NetClientCollector::defaultSimulationThreadFunc(NetClientCollector *self) {
    while (true) {
        int readyClientIdx = self->getFirstReadySocketIdx();
        if (isListener(readyClientIdx)) {
            self->acceptNewClient();
            continue;
        }
        try {
            sf::TcpSocket &client = self->getClient(readyClientIdx);
            net::NetMessage message = receiveNetMessage(client);
            self->handleNetMessage(client, message);
        } catch (const ClientReceiveException &exception) {
            if (exception.getStatus() == sf::Socket::Disconnected) {
                self->disconnectClient(readyClientIdx);
                LOGD << "Client was disconnected successfully";
                continue;
            }
            throw ClientReceiveException("Unexpected client receive exception status", exception.getStatus());
        }
    }
}

NetClientCollector::NetClientCollector(std::function<void()> simulationThread) :
        listener(),
        ipAddress(),
        simulationThread(std::move(simulationThread)) {}

NetClientCollector::NetClientCollector() :
        NetClientCollector([this]() { defaultSimulationThreadFunc(this); }) {}

NetClientCollector::~NetClientCollector() {
    for (auto &client: clients) {
        client->disconnect();
    }
    listener.close();
}

void NetClientCollector::acceptNewClient() {
    clients.emplace_back(std::make_unique<sf::TcpSocket>());
    sf::TcpSocket &newClient = *clients.back();
    if (listener.accept(newClient) != sf::Socket::Done) {
        LOGD << "Cannot accept new client";
        throw NetworkException("Cannot accept new client");
    }
    LOGD << "New client was accepted";
}

void NetClientCollector::disconnectClient(int idx) {
    if (idx < 0 || idx >= clients.size()) {
        throw std::invalid_argument("Client index is out of range");
    }
    sf::TcpSocket &client = *clients[idx];
    client.disconnect();
    LOGD << "Client " << idx << " was successfully disconnected";
    if (idx != clients.size() - 1) {
        clients[idx] = std::move(clients.back());
    }
    clients.resize(clients.size() - 1);
}

/**
 * @return index of first ready socket (-1 means that it is socket listener)
 */
int NetClientCollector::getFirstReadySocketIdx() {
    // TODO not allow one socket to take the whole CPU time
    LOGD << "Wait for the first ready socket";
    sf::SocketSelector socketSelector;
    for (auto &client: clients) {
        socketSelector.add(*client);
    }
    socketSelector.add(listener);
    socketSelector.wait();
    if (socketSelector.isReady(listener)) {
        return -1;
    };
    for (int idx = 0; idx < clients.size(); idx++) {
        if (socketSelector.isReady(*clients[idx])) {
            return idx;
        };
    }
    throw NetworkException("Unexpected socket selector case");
}

void NetClientCollector::listen(sf::IpAddress address, const std::vector<uint16_t> &ports) {
    ipAddress = address;
    for (const auto port: ports) {
        if (listener.listen(port, ipAddress) == sf::Socket::Done) {
            LOGD << "Successfully bind: " << ipAddress.toString() << " " << port;
            return;
        }
        LOGD << "Port " << port << " was unsuccessfully to bound, try next";
    }
    throw NetworkException("Cannot bind to socket");
}

void NetClientCollector::listen(sf::IpAddress address, uint16_t port) {
    listen(address, std::vector({port}));
}

void NetClientCollector::launch() {
    simulationThread.launch();
    LOGD << "New NetClientCollector was launched for new incoming connections";
}

uint32_t NetClientCollector::getIP() {
    return ipAddress.toInteger();
}

uint16_t NetClientCollector::getPort() const {
    return listener.getLocalPort();
}

bool NetClientCollector::isListener(int idx) {
    return idx == -1;
}

sf::TcpSocket &NetClientCollector::getClient(int idx) {
    return *(clients[idx]);
}
