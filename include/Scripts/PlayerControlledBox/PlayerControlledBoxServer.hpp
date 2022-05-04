#pragma once

#include <Script.hpp>

namespace null {

    class PlayerControlledBoxServer : public Script {
    public:
        void start() override;
        void update() override;
        explicit PlayerControlledBoxServer(GameObject& go) : Script(go) {}

    private:
        bool isMoving = false;
    };

}
