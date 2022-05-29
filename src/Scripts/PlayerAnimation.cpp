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
        if (controlled && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            gameObject.getRigidBody()->SetLinearVelocity({3.0f, gameObject.getRigidBody()->GetLinearVelocity().y});
            moovin = true;
            if (spriteSheet.currAnimation->name != "walkRight") {
                fram = 0;
                flip = false;
                spriteSheet.setAnimation("walkRight");
            }
        }
        if (controlled && sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !moovin) {
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
            if (!moovin or std::abs(previousPosition.x - gameObject.getPosition().x) > 30) {
                spriteSheet.setFrame(
                        spriteSheet.currFrame == spriteSheet.currAnimation->end ? 0 : spriteSheet.currFrame + 1);
                previousPosition = gameObject.getPosition();
            }
        }

        if (controlled && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canJump) {
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
    ) : RigidBodyAnimation(gameObject, spriteSheet, map) {}

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

    void PlayerAnimation::damage(float healthDelta) {
        health -= healthDelta;
        if (health < 0) {
            health = 0;
            gameObject.destroy();
        }
    }

    std::shared_ptr<GameObject> PlayerAnimation::initPlayer(const std::string& anim, b2World& box2dWorld) {
        auto player = std::make_shared<GameObject>();
        player->addTag("player");
        player->getSprite().setTextureRect({0, 0, 30, 54});
        player->getSprite().setScale(3.0f, 3.0f);
        player->visible = true;
        player->renderLayer = serial::FOREGROUND1;
        player->makeDynamic(box2dWorld);
        player->getRigidBody()->SetFixedRotation(true);
        auto playerSpriteSheet = SpriteSheet(anim, {30, 54}, {{"idle",      0, 0, 7},
                                                              {"walkRight", 1, 0, 3},
                                                              {"walkLeft",  2, 0, 3}});

        auto shape1 = new b2PolygonShape();
        auto sizeVector = Utility::pixelToMetersVector(sf::Vector2i{60, 162});
        shape1->SetAsBox(sizeVector.x / 2, sizeVector.y / 2, player->getRigidBody()->GetLocalCenter(), 0.0f);
        b2FixtureDef fixtureDef1;
        fixtureDef1.shape = shape1;
        fixtureDef1.density = 1;
        fixtureDef1.friction = 0;

        auto shape2 = new b2PolygonShape();
        sizeVector = Utility::pixelToMetersVector(sf::Vector2i{90, 162});
        shape2->SetAsBox(sizeVector.x / 2, sizeVector.y / 2, player->getRigidBody()->GetLocalCenter(), 0.0f);
        b2FixtureDef fixtureDef2;
        fixtureDef2.shape = shape2;
        fixtureDef2.density = 1;
        fixtureDef2.friction = 0;

        auto shape3 = new b2PolygonShape();
        sizeVector = Utility::pixelToMetersVector(sf::Vector2i{78, 162});
        shape3->SetAsBox(sizeVector.x / 2, sizeVector.y / 2, player->getRigidBody()->GetWorldCenter(), 0.0f);
        b2FixtureDef fixtureDef3;
        fixtureDef3.shape = shape3;
        fixtureDef3.density = 1;


        player->addScript<PlayerAnimation>(*player, playerSpriteSheet,
                                           CollisionMap({
                                                                {"idle",      {{fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}}},
                                                                {"walkRight", {{fixtureDef1}, {fixtureDef2}, {fixtureDef1}, {fixtureDef2}}},
                                                                {"walkLeft",  {{fixtureDef1}, {fixtureDef2}, {fixtureDef1}, {fixtureDef2}}}
                                                        }));
        return player;
    }

    GameObject& PlayerAnimation::getGameObject() {
        return gameObject;
    }

}
