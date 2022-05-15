#include "PlayerAnimation.hpp"
#include "Serializer.hpp"

#include <Utility.hpp>
#include <utility>
#include <iostream>
#include <MainLoop.hpp>
#include "Physics/CollisionCategories.hpp"

namespace null {
    void PlayerAnimation::start() {
        spriteSheet.setAnimation("walkRight");
        RigidBodyAnimation::update();
    }

    void PlayerAnimation::update() {
        static bool moovin = false;
        static int fram = 0;
        static bool canJump = false;

        auto rb = gameObject.getRigidBody();
        if (!canJump) {
            for (auto* contact = rb->GetContactList(); contact != nullptr; contact = contact->next) {
                if (!contact->contact->IsTouching())
                    continue;

                auto* otherRb = contact->other;
                auto* otherGo = (GameObject*) otherRb->GetUserData().pointer;
                if (otherRb->GetWorldCenter().y - Utility::pixelToMetersVector<float>(
                        {0, otherGo->getSprite().getTextureRect().height * otherGo->getSprite().getScale().y}).y / 2 +
                    1 >
                    rb->GetWorldCenter().y + Utility::pixelToMetersVector<float>({0,
                                                                                  gameObject.getSprite().getTextureRect().height *
                                                                                  gameObject.getSprite().getScale().y}).y /
                                             2) {
                    canJump = true;
                    rb->SetLinearVelocity({rb->GetLinearVelocity().x, 0});
                    break;
                }
            }
        }

        moovin = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            gameObject.getRigidBody()->SetLinearVelocity({3.0f, gameObject.getRigidBody()->GetLinearVelocity().y});
            moovin = true;
            if (spriteSheet.currAnimation->name != "walkRight") {
                fram = 0;
                flip = false;
                spriteSheet.setAnimation("walkRight");
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !moovin) {
            gameObject.getRigidBody()->SetLinearVelocity({-3.0f, gameObject.getRigidBody()->GetLinearVelocity().y});
            moovin = true;
            if (spriteSheet.currAnimation->name != "walkLeft") {
                fram = 0;
                flip = true;
                spriteSheet.setAnimation("walkLeft");
            }
        }

        if (!moovin) {
            if (!(spriteSheet.currAnimation->name == "idle")) {
                spriteSheet.setAnimation("idle");
                fram = 0;
            }
            gameObject.getRigidBody()->SetLinearVelocity({0, gameObject.getRigidBody()->GetLinearVelocity().y});
        }

        fram++;
        if (fram >= 3) {
            fram = 0;
            spriteSheet.setFrame(
                    spriteSheet.currFrame == spriteSheet.currAnimation->end ? 0 : spriteSheet.currFrame + 1);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canJump) {
            gameObject.getRigidBody()->SetLinearVelocity({gameObject.getRigidBody()->GetLinearVelocity().x, -6});
            canJump = false;
        }
        RigidBodyAnimation::update();
        if (gameObject.getRigidBody()->GetFixtureList() != nullptr) {
            gameObject.setCollisionCategories(PLAYER_CATEGORY);
            gameObject.setCollisionMasks(ALL_CATEGORIES & ~GRENADE_CATEGORY);
        }
    }

    PlayerAnimation::PlayerAnimation(GameObject& gameObject,
                                     SpriteSheet& spriteSheet,
                                     const CollisionMap& map
                                     ) : RigidBodyAnimation(gameObject, spriteSheet, map) { }

    void PlayerAnimation::serialize(google::protobuf::Message& message) const {
        auto& msg = dynamic_cast<serial::Script&>(message);
        auto s_anim = msg.mutable_player_animation();
        auto s_ss = s_anim->mutable_animation()->mutable_spritesheet();
        spriteSheet.serialize(*s_ss);
        auto cm = s_anim->mutable_animation()->mutable_collision_map();
        collisionMap.serialize(*cm);
    }

    std::unique_ptr<Component> PlayerAnimation::deserialize(const google::protobuf::Message& message) {
        auto& msg = dynamic_cast<const serial::Script&>(message);
        auto const& s_anim = msg.player_animation();
        auto const& s_ss = s_anim.animation().spritesheet();
        auto const& cm = s_anim.animation().collision_map();
        auto p_ss = SpriteSheet::deserialize(s_ss);
        auto p_cm = CollisionMap::deserialize(cm);
        return std::make_unique<PlayerAnimation>(
                *Serializer::currentDeserializationGameObject,
                *p_ss,
                *p_cm
                );
    }

}
