#pragma once

#include <RigidBodyAnimation.hpp>

namespace null {
    class PlayerAnimation : public RigidBodyAnimation {
    public:
        bool flip = false;
        void start() override;
        void update() override;
        PlayerAnimation(GameObject&, SpriteSheet&, const std::unordered_map<std::string, std::vector<std::vector<b2FixtureDef>>>&);
    };

}

