#pragma once

#include "Script.hpp"
#include "SFML/Graphics.hpp"
#include "Schedulers/Timer.hpp"
#include "RigidBodyAnimation.hpp"

namespace null {
    class GrenadeScript : public RigidBodyAnimation{
    private:
        Timer timer{std::chrono::milliseconds{3000}};
        Timer frameChangeTimer{std::chrono::milliseconds{1500}};
        float speed, angle;
        sf::Vector2f from;
    public:
        GameObject* contactedGameObject = nullptr;

        void start() override;

        GrenadeScript(GameObject& object,
                      SpriteSheet& spriteSheet,
                      const CollisionMap& collisionMap,
                      float speed,
                      float angle, sf::Vector2f from);


        void update() override;
    };
}
