#pragma once

#include "Script.hpp"

namespace null {

    class PlayerControlledBoxClient : public Script {
    public:
        void start() override;
        void update() override;
        explicit PlayerControlledBoxClient(GameObject& go) : Script(go) {}

    private:
        bool isMoving = false;
    };

}
