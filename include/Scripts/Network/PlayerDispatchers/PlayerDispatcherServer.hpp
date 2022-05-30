#pragma once

#include <queue>
#include <network.h>
#include <memory>

#include <Script.hpp>
#include <client/ClientNetworkManager.h>
#include <Network/NetworkManagerServerScript.hpp>
#include <Network/Utility/EveryAcceptedMessageProcessor.hpp>

namespace null {

    /**
     * Monitors available players (not connected)
     * Echos to client dispatchers the next available one
     * When receives a message that someone has taken it, echos the next one
     */
    class PlayerDispatcherServer: public Script {
    public:

        void start() override;
        void update() override;
        explicit PlayerDispatcherServer(GameObject& go);

    private:
        EveryAcceptedMessageProcessor<net::GameMessage::ClientCommand> messageQueue{};
        uint32_t currentPlayerIdx = 0;
        std::vector<std::string> players{"player1", "player2", "player3", "player4"};
        std::unique_ptr<NetworkManagerServerScript> networkManager{};
    };

}
