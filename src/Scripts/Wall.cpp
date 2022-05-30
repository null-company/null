//
// Created by artemonchelus on 29.05.22.
//

#include "Wall.hpp"
#include "GameObject.hpp"
#include "PlayerAnimation.hpp"
#include "Serializer.hpp"

namespace null {
    void Wall::start() {
        Component::start();
    }

    void Wall::update() {
        auto players = gameObject.getContactedGameObjects<PlayerAnimation>();
        for (auto& player: players) {
            auto pos = player->getGameObject().getPosition();
            player->getGameObject().setPosition(pos.x, 0);
            player->getGameObject().getRigidBody()->SetLinearVelocity({0,0});
        }
        Component::update();
    }

    Wall::Wall(GameObject& object) : Component(object) {}

    void Wall::serialize(google::protobuf::Message& message) const {
        auto& msg = dynamic_cast<serial::Script&>(message);
        msg.mutable_wall();
    }

    std::unique_ptr<Component> Wall::deserialize(const google::protobuf::Message& message) {
        auto& msg = dynamic_cast<const serial::Script&>(message);
        return std::make_unique<Wall>(
                *Serializer::currentDeserializationGameObject
        );
    }
} // null