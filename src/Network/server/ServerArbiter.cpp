#include "server/ServerArbiter.h"
#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"


net::GameServerConfig ServerArbiter::createNewGameSimulation() {
    net::GameServerConfig gameServerConfig;
    
}

ServerArbiter::ServerArbiter(sf::IpAddress ipAddress, uint16_t port) : ClientCollector(ipAddress, port) {}
