#pragma once

#include <queue>
#include <network.h>

#include <Script.hpp>
#include <Network/NetworkManagerClientScript.hpp>
#include <Network/NetworkStateManager.hpp>

namespace null {

    class PlayerControlledBoxClient : public Script {
    public:
        void start() override;
        void update() override;
        explicit PlayerControlledBoxClient(GameObject& go);

    private:
        float x{}, y{};
        NetworkStateManager networkStateManager{x, y}; // network state manager will be restoring these values
        std::queue<net::GameMessage::SubscriberState>* messageQueue = nullptr;
        NetworkManagerClientScript* networkManagerScript{};
    };

}
