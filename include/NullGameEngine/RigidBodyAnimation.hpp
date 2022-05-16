#pragma once

#include <Animation.hpp>
#include <GameObject.hpp>
#include "CollisionMap.hpp"

namespace null{

    class RigidBodyAnimation : public Animation {
    public:

        CollisionMap collisionMap;

        std::string currRigidAnim;

        int currRigidFrame = 0;

        void destroyAllFixtures();

        void createFixtures();

        void update() override;

        // Creates RigidBodyAnimation
        // Throws exception if provided map and sprite sheet don't line up
        // NOTE: all user data in fixture defs will be lost in the process
        RigidBodyAnimation(GameObject& gameObject, SpriteSheet& spriteSheet,
                           const CollisionMap & collisionMap);

        void serialize(google::protobuf::Message&) const override;
        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);

    };
}

