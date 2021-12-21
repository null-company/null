#include "PlayerAnimation.hpp"

#include <utility>
#include <iostream>

namespace  null {
    void PlayerAnimation::start() {
        spriteSheet.setAnimation("walkRight");
        RigidBodyAnimation::update();
    }

    void PlayerAnimation::update() {
        static bool moovin = false;
        static int fram = 0;

        moovin = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            gameObject.getRigidBody()->SetLinearVelocity({3.0f, gameObject.getRigidBody()->GetLinearVelocity().y});
            moovin = true;
            if (flip) {
                flip = false;
                spriteSheet.setAnimation("walkRight");
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !moovin) {
            gameObject.getRigidBody()->SetLinearVelocity({-3.0f, gameObject.getRigidBody()->GetLinearVelocity().y});
            moovin = true;
            if (!flip) {
                flip = true;
                spriteSheet.setAnimation("walkLeft");
            }
        }
        if (!moovin) {
            gameObject.getRigidBody()->SetLinearVelocity({0, gameObject.getRigidBody()->GetLinearVelocity().y});
        }
        if (moovin) {
            fram++;
            if (fram >= 3) {
                fram = 0;
                spriteSheet.setFrame(spriteSheet.currFrame == spriteSheet.currAnimation->end ? 0 : spriteSheet.currFrame + 1);
            }

        }
        //TODO
        //used for debug log, will remove later
//        for (auto& xxx : fixtureMap) {
//            std::cout << xxx.first << std::endl;
//            for (auto& ddd : xxx.second) {
//                for (auto fff : ddd) {
//                    std::cout << (fff->GetFilterData().maskBits != 0) << ' ';
//                }
//                std::cout << std::endl;
//            }
//        }
//        std::cout << gameObject.getPosition().x << ", " << gameObject.getPosition().y << std::endl;
        RigidBodyAnimation::update();
    }

    PlayerAnimation::PlayerAnimation(GameObject& gameObject, SpriteSheet& spriteSheet,
                                     const std::unordered_map<std::string, std::vector<std::vector<b2FixtureDef>>>& map) :
            RigidBodyAnimation(gameObject, spriteSheet, map){ }
}
