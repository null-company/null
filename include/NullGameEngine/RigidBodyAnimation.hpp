#pragma once

#include <Animation.hpp>
#include <GameObject.hpp>

namespace null{
    class RigidBodyAnimation : public Animation {
    public:
        std::unordered_map<std::string, std::vector<std::vector<b2Fixture*>>> fixtureMap;

        static void enableFixture(b2Fixture* fixture);

        static void disableFixture(b2Fixture* fixture);

        void update() override;

        // Creates RigidBodyAnimation
        // Throws exception if provided map and sprite sheet don't line up
        // NOTE: all user data in fixture defs will be lost in the proccess
        // TODO: add option to pass user data
        RigidBodyAnimation(GameObject& gameObject, SpriteSheet& spriteSheet,
                           const std::unordered_map<std::string, std::vector<std::vector<b2FixtureDef>>>& fixtureDefMap);

    };
}

