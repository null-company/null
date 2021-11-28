#include "ServerArbiter.h"
#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"


ServerArbiter::ServerArbiter(sf::IpAddress ipAddress, uint16_t port) {

    if (listener.listen(port, ipAddress) != sf::Socket::Done) {
        PLOG_DEBUG << "Fuck";
    };
}
