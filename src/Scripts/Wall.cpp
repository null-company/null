//
// Created by artemonchelus on 29.05.22.
//

#include "Wall.hpp"
#include "GameObject.hpp"
#include "PlayerAnimation.hpp"

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
        Component::serialize(message);
    }
} // null