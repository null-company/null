#pragma once

#include <queue>
#include <network.h>
#include <memory>

#include <Script.hpp>
#include <client/ClientNetworkManager.h>
#include <Network/NetworkManagerClientScript.hpp>

namespace null {

    /**
     * On start, finds
     * Then listens for the first echo of an available player to make it controllable
     * and all other players uncontrollable (synced)
     */
    class PlayerDispatcherClient: public Script {
    public:
        sf::IpAddress ipToConnectTo{};
        uint16_t port{};

        void start() override;
        void update() override;
        explicit PlayerDispatcherClient(GameObject& go);

    private:
        std::shared_ptr<NetworkManagerClientScript> networkManager{};
    };
}
