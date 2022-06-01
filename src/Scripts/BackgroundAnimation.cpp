//
// Created by artemonchelus on 01.06.22.
//

#include "BackgroundAnimation.hpp"
#include <GameObject.hpp>

namespace null {

    void BackgroundAnimation::start() {
        Component::start();
        spriteSheet.setFrame(0);
        spriteSheet.setAnimation("0");
        animation = "0";
        timer.start();
    }

    void BackgroundAnimation::update() {
        Animation::update();
        if (timer.expired()) {
            if (spriteSheet.currFrame + 1 == spriteSheet.animations[animation].framePositions.size()) {
                animation = std::to_string((std::stoi(animation) + 1) % spriteSheet.animations.size());
                spriteSheet.setAnimation(animation);
            } else {
                spriteSheet.setFrame(spriteSheet.currFrame + 1);
            }
            timer.start();
        }
    }

    void BackgroundAnimation::serialize(google::protobuf::Message& message) const {
        Animation::serialize(message);
    }

    BackgroundAnimation::BackgroundAnimation(GameObject& object, const SpriteSheet& sheet) : Animation(object, sheet) {}

} // null