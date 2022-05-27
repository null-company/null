#pragma once

#include "Script.hpp"
#include "SFML/Graphics.hpp"
#include "Schedulers/Timer.hpp"
#include "RigidBodyAnimation.hpp"

namespace null {
    class GrenadeScript : public RigidBodyAnimation {
    private:
        Timer timer{std::chrono::milliseconds{3000}};
        float speed, angle;
        sf::Vector2f from;
    public:
        void start() override;

        GrenadeScript(GameObject& object, float speed, float angle, sf::Vector2f from);

        void update() override;
    };
}
