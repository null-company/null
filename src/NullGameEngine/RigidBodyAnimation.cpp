#include "RigidBodyAnimation.hpp"
#include "Serializer.hpp"
#include <scripts.pb.h>

namespace null {
    void RigidBodyAnimation::update() {
        auto currAnim = spriteSheet.currAnimation->name;
        auto currFrame = spriteSheet.currFrame;
        if (currAnim != currRigidAnim || currFrame != currRigidFrame) {
            currRigidAnim = currAnim;
            currRigidFrame = currFrame;
            destroyAllFixtures();
            createFixtures();
        }

        Animation::update();
    }

    RigidBodyAnimation::RigidBodyAnimation(GameObject& gameObject, SpriteSheet& spriteSheet,
                                           const CollisionMap& collisionMap1) :
            Animation(gameObject, spriteSheet), collisionMap(collisionMap1) {
        // check that spriteSheet and collisionMap are the same size
        if (collisionMap.collisionMapInternal.size() != spriteSheet.animations.size()) {
            throw std::invalid_argument("RigidBodyAnimation error: collisionMap and spriteSheet are not the same size");
        }
        for (const auto& [name, vec]: collisionMap.collisionMapInternal) {
            if (vec.size() != spriteSheet.animations[name].framePositions.size()) {
                throw std::invalid_argument(
                        "RigidBodyAnimation error: collisionMap and spriteSheet are not the same size");
            }
        }

        destroyAllFixtures();

        for (const auto& pair: collisionMap.collisionMapInternal) {
            this->collisionMap.collisionMapInternal.insert(pair);
        }
    }

    void RigidBodyAnimation::destroyAllFixtures() {
        for (auto fx = gameObject.getRigidBody()->GetFixtureList();
             fx != nullptr; fx = gameObject.getRigidBody()->GetFixtureList()) {
            gameObject.getRigidBody()->DestroyFixture(fx);
        }
    }

    void RigidBodyAnimation::createFixtures() {
        for (auto const& fd: collisionMap.collisionMapInternal[currRigidAnim][currRigidFrame]) {
            gameObject.getRigidBody()->CreateFixture(&fd);
        }
    }

    void RigidBodyAnimation::serialize(google::protobuf::Message& message) const {
        auto& msg = dynamic_cast<serial::Script&>(message);
        auto s_rba = msg.mutable_rigid_body_animation();
        auto s_ss = s_rba->mutable_spritesheet();
        spriteSheet.serialize(*s_ss);
        auto s_cm = s_rba->mutable_collision_map();
        collisionMap.serialize(*s_cm);
    }

    std::unique_ptr<Component> RigidBodyAnimation::deserialize(const google::protobuf::Message& message) {
        auto& msg = dynamic_cast<const serial::Script&>(message);
        auto& s_script = msg.rigid_body_animation();
        auto spsh = SpriteSheet::deserialize(s_script.spritesheet());
        auto cm = CollisionMap::deserialize(s_script.collision_map());
        return std::make_unique<RigidBodyAnimation>(
                *Serializer::currentDeserializationGameObject,
                *spsh,
                *cm
        );
    }
}
