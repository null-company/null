#pragma once

#include <queue>
#include <network.h>

#include <Script.hpp>
#include <Network/NetworkManagerClientScript.hpp>
#include <Network/AbstractNetworkStateWrapper.hpp>
#include <Network/Utility/LastAcceptedMessageProcessor.hpp>

namespace null {

    class PlayerControlledBoxClient : public Script {
    public:
        void start() override;
        void update() override;
        explicit PlayerControlledBoxClient(GameObject& go);

    private:
        float x{}, y{};
        NetworkStateManager networkStateManager{x, y}; // network state manager will be restoring these values
        LastAcceptedMessageProcessor<net::GameMessage::SubscriberState> messageQueue{};
        NetworkManagerClientScript* networkManagerScript{};
    };

}
