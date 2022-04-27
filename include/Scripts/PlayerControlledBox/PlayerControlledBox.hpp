#pragma once

#include "Script.hpp"

namespace null {

    class PlayerControlledBox : public Script {
    public:
        void start() override;
        void update() override;
        explicit PlayerControlledBox(GameObject& go) : Script(go) {}

    private:
        bool isMoving = false;
    };

}
