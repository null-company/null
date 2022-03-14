#pragma once
#include <Script.hpp>

namespace null{
    class BulletScript: public Script{
    public:
        BulletScript(GameObject& object);

        void start() override;

        void update() override;
    };
}
