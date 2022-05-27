#pragma once

#include <RigidBodyAnimation.hpp>

namespace null {
    /**
     * :field controlled - set true if player must be controlled by keyboard, false otherwise
     */
    class PlayerAnimation : public RigidBodyAnimation {
    public:
        bool flip = false;
        bool controlled = false;

        void start() override;

        void update() override;

        PlayerAnimation(GameObject&, SpriteSheet&,
                        const std::unordered_map<std::string, std::vector<std::vector<b2FixtureDef>>>&);
    };

}

