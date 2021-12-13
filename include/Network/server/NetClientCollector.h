#ifndef NULL_GAME_NETCLIENTCOLLECTOR_H
#define NULL_GAME_NETCLIENTCOLLECTOR_H

#include <SFML/Network.hpp>
#include <cstdint>
#include <list>
#include <serverConfig.pb.h>

class NetClientCollector {
private:
    sf::TcpListener listener;

protected:
    sf::Thread simulationThread;

    sf::IpAddress ipAddress;

    static void defaultSimulationThreadFunc(NetClientCollector *clientCollector);

    std::vector<std::unique_ptr<sf::TcpSocket>> clients;
public:

    NetClientCollector();

    int getFirstReadySocketIdx();

    sf::TcpSocket &getClient(int idx);

    virtual void acceptNewClient();

    virtual void handleNetMessage(int clientIdx, const net::NetMessage &message) = 0;

    NetClientCollector(std::function<void()> simulationThread);

    virtual ~NetClientCollector();

    void launch();

    uint32_t getIP();

    uint16_t getPort() const;

    virtual void disconnectClient(int idx);

    void listen(sf::IpAddress address, const std::vector<uint16_t> &ports);

    void listen(sf::IpAddress address, uint16_t port);

    static bool isListener(int idx);
};

#endif //NULL_GAME_NETCLIENTCOLLECTOR_H
