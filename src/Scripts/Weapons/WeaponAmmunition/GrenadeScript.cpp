#include "Weapons/WeaponAmmunition/GrenadeScript.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Graphic/Vector.hpp"
#include "ResourceManager.hpp"
#include "Physics/CollisionCategories.hpp"
#include "PlayerAnimation.hpp"

namespace null {
    void null::GrenadeScript::start() {
        spriteSheet.setAnimation("");
        spriteSheet.setFrame(0);
        gameObject.setPosition(from);
        RigidBodyAnimation::start();
        gameObject.getRigidBody()->SetLinearVelocity(
                {cos(2 * 3.14f / 360 * angle) * speed, sin(2 * 3.14f / 360 * angle) * speed});
        frameChangeTimer.start();
    }

    void null::GrenadeScript::update() {
        if (frameChangeTimer.expired()) {
            int step = spriteSheet.currFrame;
            int explodeStep = 5;
            if (step == 0) {
                frameChangeTimer = Timer(std::chrono::milliseconds(100));
            }
            if (step == explodeStep) {
                gameObject.getRigidBody()->SetType(b2_staticBody);
            }
            if (step == explodeStep +1 ) {
                auto players = gameObject.getContactedGameObjects<PlayerAnimation>();
                for (auto player: players) {
                    player->damage(30);
                }
            }
            frameChangeTimer.start();
            spriteSheet.setFrame(spriteSheet.currFrame + 1);
            if (collisionMap.collisionMapInternal[""].size() == spriteSheet.currFrame + 1) {
                gameObject.destroy();
                return;
            }
        }

        RigidBodyAnimation::update();
    }

    GrenadeScript::GrenadeScript(GameObject& object,
                                 SpriteSheet& spriteSheet,
                                 const CollisionMap& collisionMap,
                                 float speed,
                                 float angle,
                                 sf::Vector2f from) : RigidBodyAnimation(object, spriteSheet, collisionMap),
                                                      speed(speed),
                                                      angle(angle),
                                                      from(from) {}

    void GrenadeScript::serialize(google::protobuf::Message& message) const {
        RigidBodyAnimation::serialize(message);
    }

    std::unique_ptr<Script> GrenadeScript::deserialize(const google::protobuf::Message& message) {
        return std::unique_ptr<Script>();
    }
}
