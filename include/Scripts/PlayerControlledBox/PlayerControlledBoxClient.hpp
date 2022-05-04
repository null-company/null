#pragma once

#include <queue>
#include <network.h>

#include "Script.hpp"

namespace null {

    class PlayerControlledBoxClient : public Script {
    public:
        void start() override;
        void update() override;
        explicit PlayerControlledBoxClient(GameObject& go) : Script(go) {}

    private:
        std::queue<net::GameMessage::SubscriberState>* messageQueue = nullptr;
        bool isMoving = false;
    };

}
