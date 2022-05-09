#pragma once

#include <queue>
#include <Script.hpp>
#include "serialized/serverConfig.pb.h"

namespace null {

    class PlayerControlledBoxServer : public Script {
    public:
        void start() override;
        void update() override;
        explicit PlayerControlledBoxServer(GameObject& go) : Script(go) {}

    private:
        std::queue<net::GameMessage::ClientCommand>* messageQueue = nullptr;
        bool isMoving = false;
    };

}