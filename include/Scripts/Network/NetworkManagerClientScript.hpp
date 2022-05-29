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
    struct NetworkManagerClientScript: public Script {
        /**
         * It is caller responsibility to set these values
         */
        sf::IpAddress ipToConnectTo{};
        uint16_t port{};

        void start() override;
        void update() override;
        explicit NetworkManagerClientScript(GameObject& go);

        void serialize(google::protobuf::Message & message) const override;

        static std::unique_ptr<Script> deserialize(const google::protobuf::Message& message);

        [[nodiscard]]
        ClientNetworkManager& getNetworkManager() const;

    private: // TODO consider making public?
        std::unique_ptr<ClientNetworkManager> networkManager{};
    };

}
