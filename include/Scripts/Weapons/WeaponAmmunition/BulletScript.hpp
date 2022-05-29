#pragma once

#include "Script.hpp"
#include "SFML/System/Vector2.hpp"
#include "Projectile.hpp"

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

        void serialize(google::protobuf::Message &) const override;
        static std::unique_ptr<BulletScript> deserialize(const google::protobuf::Message&);
    };
}
