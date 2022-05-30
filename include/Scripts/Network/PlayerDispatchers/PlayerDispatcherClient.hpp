#pragma once

#include <queue>
#include <network.h>
#include <memory>

#include <Script.hpp>
#include <client/ClientNetworkManager.h>
#include <Network/NetworkManagerClientScript.hpp>
#include <Network/Utility/LastAcceptedMessageProcessor.hpp>

namespace null {

    /**
     * Listens for server dispatcher messages to find available player tag.
     * Then makes the player controllable and all other players uncontrollable (synced)
     * And send a message to the server dispatcher
     */
    class PlayerDispatcherClient: public Script {
    public:

        void start() override;
        void update() override;
        explicit PlayerDispatcherClient(GameObject& go);

    private:
        bool foundPlayer = false;
        LastAcceptedMessageProcessor<net::GameMessage::SubscriberState> messageQueue{};
        NetworkManagerClientScript* networkManagerScript{};
    };
}
