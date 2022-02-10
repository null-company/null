#pragma once

#include <Animation.hpp>
#include <GameObject.hpp>

namespace null{
    class RigidBodyAnimation : public Animation {
    public:

        std::unordered_map<std::string, std::vector<std::vector<b2FixtureDef>>> fixtureDefMap;

        std::string currRigidAnim;

        int currRigidFrame = 0;

        void destroyAllFixtures();

        void createFixtures();

        void update() override;

        // Creates RigidBodyAnimation
        // Throws exception if provided map and sprite sheet don't line up
        // NOTE: all user data in fixture defs will be lost in the proccess
        RigidBodyAnimation(GameObject& gameObject, SpriteSheet& spriteSheet,
                           const std::unordered_map<std::string, std::vector<std::vector<b2FixtureDef>>>& fixtureDefMap);


    };
}

