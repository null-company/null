#pragma once

#include <Script.hpp>

namespace null {
    class DeleteScript : public Script {
    private:
        bool isDeleted = false;
    public:
        DeleteScript(GameObject& object);

        void start() override;

        void update() override;

        void del();
    };
}
