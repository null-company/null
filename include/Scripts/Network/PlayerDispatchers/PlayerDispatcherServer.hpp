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
        std::vector<std::string> players;
    public:

        void start() override;
        void update() override;
        explicit PlayerDispatcherServer(GameObject& go);

    private:
        bool isFinished = false;
        EveryAcceptedMessageProcessor<net::GameMessage::ClientCommand> messageQueue{};
        uint32_t currentPlayerIdx = 0;
    };

}
