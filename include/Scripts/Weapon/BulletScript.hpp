#pragma once

#include <Script.hpp>
#include "SFML/System/Vector2.hpp"

namespace null {
    class BulletScript : public Script {
    private:
        sf::Vector2f from;
        float rotation;
        float speed;
    public:
        GameObject* contactedGameObject = nullptr;

        BulletScript(GameObject& object, sf::Vector2f from, float rotation, float speed);

        void start() override;

        void update() override;
    };
}
