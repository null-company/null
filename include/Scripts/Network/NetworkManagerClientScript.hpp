#pragma once

#include <queue>
#include <network.h>
#include <memory>

#include <Script.hpp>
#include <client/ClientNetworkManager.h>

namespace null {

    /**
     * A place to manage any network deeds.
     * You may want this script to run first. TODO investigate this issue
     */
    class NetworkManagerClientScript : public Script {
    public:
        /**
         * It is caller responsibility to set these values
         */
        sf::IpAddress serverArbiterIp{};
        uint16_t serverArbiterPort{};

        void start() override;
        void update() override;
        explicit NetworkManagerClientScript(GameObject& go);

        [[nodiscard]]
        ClientNetworkManager& getNetworkManager() const;

    private: // TODO consider making public?
        std::unique_ptr<ClientNetworkManager> networkManager{};
    };

}
