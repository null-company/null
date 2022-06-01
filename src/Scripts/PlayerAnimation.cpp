#include "PlayerAnimation.hpp"
#include "Serializer.hpp"
#include <Network/NetworkManagerServerScript.hpp>

#include <ResourceManager.hpp>
#include <Utility.hpp>
#include <utility>
#include <MainLoop.hpp>
#include <Physics/CollisionCategories.hpp>

namespace null {
    void PlayerAnimation::start() {
        deathSound = &ResourceManager::getSound("death.ogg");
        jumpSound = &ResourceManager::getSound("jump.ogg");
        spriteSheet.setAnimation("walkRight");
        RigidBodyAnimation::update();

        host = MainLoop::serverArbiter == nullptr ? Client : Server;
        if (host == Client) {
            auto networkManagerObject =
                    gameObject.getScene().lock()->findFirstByTag("network-manager");
            networkManagerScript = networkManagerObject.lock()->getScript<NetworkManagerClientScript>();
            clientQueue.attachTo(&networkManagerScript->getNetworkManager().subscribe(gameObject.guid));
        } else {
            serverQueue.attachTo(
                    &gameObject.getScene().lock()->findFirstByTag("network-manager")
                            .lock()->getScript<NetworkManagerServerScript>()->subscribe(gameObject.guid)
            );
        }
    }

    namespace {
        constexpr float movingVelocity = 3.0f;
        constexpr float jumpVelocity = 6.0f;
    }

    void PlayerAnimation::update() {
        auto rb = gameObject.getRigidBody();

        if (host == Client) {
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
        }

        bool playerIsMoving = false;
        if (controller == Keyboard) {
            // it is assumed that host == Client
            auto commands = getCurrentCommands();
            for (const auto command : commands) {
                if (command == LEFT || command == RIGHT) {
                    playerIsMoving = true;
                }
                processCommand(command);
            }
        }
        if (host == Server) {
            if (controller == Keyboard) {
                throw std::logic_error("impossible");
            }
            serverQueue.processMessageIfAny([this, &playerIsMoving](net::GameMessage::ClientCommand& commandMessage) {
                KeyboardCommand direction;
                CommandConverter::restoreFromMessage(commandMessage.content(),
                                                     reinterpret_cast<uint32_t&>(direction));
                if (direction == RIGHT) {
                    gameObject.getRigidBody()->SetLinearVelocity(
                            {movingVelocity,gameObject.getRigidBody()->GetLinearVelocity().y}
                    );
                    playerIsMoving = true;
//                        if (spriteSheet.currAnimation->name != "walkRight") {
//                            currentAnimationFrame = 0;
//                            spriteSheet.setAnimation("walkRight");
//                        }
                }
                if (direction == LEFT && !playerIsMoving) {
                    gameObject.getRigidBody()->SetLinearVelocity({-movingVelocity, gameObject.getRigidBody()->GetLinearVelocity().y});
                    playerIsMoving = true;
//                        if (spriteSheet.currAnimation->name != "walkLeft") {
//                            currentAnimationFrame = 0;
//                            spriteSheet.setAnimation("walkLeft");
//                        }
                }
                if (direction == JUMP) {
                    gameObject.getRigidBody()->SetLinearVelocity(
                            {gameObject.getRigidBody()->GetLinearVelocity().x, -jumpVelocity}
                    );
                }
            });
            const auto& position = gameObject.getPosition();
            const auto& velocity = gameObject.getRigidBody()->GetLinearVelocity();
            net::GameMessage::SubscriberState stateToBroadcast;
            stateToBroadcast.set_subscriber_id(gameObject.guid);
            *stateToBroadcast.mutable_content() =
                    StateConverter::makeMessageFrom(position.x, position.y, velocity.x, velocity.y);
            MainLoop::serverArbiter->getGameServer().broadcastMessage(
                    stateToBroadcast
            );
        } else {
            // controller == Network OR Keyboard
            if (host != Client) {
                throw std::logic_error("impossible");
            }
            clientQueue.processMessageIfAny([this, &playerIsMoving](net::GameMessage::SubscriberState& message) {
                struct {
                    float x, y;
                } playerPos{}, velocity{};
                PrimitiveStateConverter::restoreFromMessage(
                        message.content(),
                        playerPos.x, playerPos.y,
                        velocity.x, velocity.y
                );
                if (velocity.x != 0) {
                    playerIsMoving = true;
                }
                if (velocity.x > 0) {
                    if (spriteSheet.currAnimation->name != "walkRight") {
                        currentAnimationFrame = 0;
                        spriteSheet.setAnimation("walkRight");
                    }
                }
                if (velocity.x < 0) {
                    if (spriteSheet.currAnimation->name != "walkLeft") {
                        currentAnimationFrame = 0;
                        spriteSheet.setAnimation("walkLeft");
                    }
                }
                gameObject.setPosition(playerPos.x, playerPos.y);
                gameObject.getRigidBody()->SetLinearVelocity({velocity.x, velocity.y});
            });
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
//                gameObject.getRigidBody()->SetLinearVelocity({movingVelocity, gameObject.getRigidBody()->GetLinearVelocity().y});
//                playerIsMoving = true;
//                if (spriteSheet.currAnimation->name != "walkRight") {
//                    currentAnimationFrame = 0;
//                    spriteSheet.setAnimation("walkRight");
//                }
//            }
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !playerIsMoving) {
//                gameObject.getRigidBody()->SetLinearVelocity({-movingVelocity, gameObject.getRigidBody()->GetLinearVelocity().y});
//                playerIsMoving = true;
//                if (spriteSheet.currAnimation->name != "walkLeft") {
//                    currentAnimationFrame = 0;
//                    spriteSheet.setAnimation("walkLeft");
//                }
//            }
        }

        if (!playerIsMoving) {
            if (!(spriteSheet.currAnimation->name == "idle")) {
                spriteSheet.setAnimation("idle");
                currentAnimationFrame = 0;
            }
            gameObject.getRigidBody()->SetLinearVelocity({0, gameObject.getRigidBody()->GetLinearVelocity().y});
        }

        currentAnimationFrame++;
        if (currentAnimationFrame >= 3) {
            currentAnimationFrame = 0;
            if (!playerIsMoving or std::abs(previousPosition.x - gameObject.getPosition().x) > 30) {
                spriteSheet.setFrame(
                        spriteSheet.currFrame == spriteSheet.currAnimation->end ? 0 : spriteSheet.currFrame + 1);
                previousPosition = gameObject.getPosition();
            }
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
            deathSound->play();
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

    float PlayerAnimation::getHealth() const {
        return health;
    }

    /**
     * @return list of all commands by pressed buttons
     */
    std::vector<PlayerAnimation::KeyboardCommand> PlayerAnimation::getCurrentCommands() {
        std::vector<KeyboardCommand> res{};
        for (const auto& kv : controls) {
            if (sf::Keyboard::isKeyPressed(kv.first)) {
                res.push_back(kv.second);
            }
        }
        return res;
    }

    void PlayerAnimation::processCommand(PlayerAnimation::KeyboardCommand command) {
        switch (command) {
            case LEFT: {
                gameObject.getRigidBody()->SetLinearVelocity(
                        {-movingVelocity, gameObject.getRigidBody()->GetLinearVelocity().y}
                );
                if (spriteSheet.currAnimation->name != "walkLeft") {
                    currentAnimationFrame = 0;
                    spriteSheet.setAnimation("walkLeft");
                }
                break;
            }
            case RIGHT: {
                gameObject.getRigidBody()->SetLinearVelocity(
                        {movingVelocity, gameObject.getRigidBody()->GetLinearVelocity().y}
                );
                if (spriteSheet.currAnimation->name != "walkRight") {
                    currentAnimationFrame = 0;
                    spriteSheet.setAnimation("walkRight");
                }
                break;
            }
            case JUMP: {
                if (!canJump) {
                    return; // do not send the command either
                }
                jumpSound->play();
                gameObject.getRigidBody()->SetLinearVelocity(
                        {gameObject.getRigidBody()->GetLinearVelocity().x, -jumpVelocity}
                );
                canJump = false;
                break;
            }
        }
        // TODO reuse message
        net::GameMessage::ClientCommand commandMessage;
        commandMessage.set_subscriber_id(gameObject.guid);
        *commandMessage.mutable_content() =
                CommandConverter::makeMessageFrom(static_cast<uint32_t>(command));
        networkManagerScript->getNetworkManager().sendCommandToServer(commandMessage);
    }

    std::shared_ptr<GameObject> ArrowsControlledPlayer::initPlayer(const std::string& anim, b2World& box2dWorld) {
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


        player->addScript<ArrowsControlledPlayer>(*player, playerSpriteSheet,
                                           CollisionMap({
                                                                {"idle",      {{fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}, {fixtureDef3}}},
                                                                {"walkRight", {{fixtureDef1}, {fixtureDef2}, {fixtureDef1}, {fixtureDef2}}},
                                                                {"walkLeft",  {{fixtureDef1}, {fixtureDef2}, {fixtureDef1}, {fixtureDef2}}}
                                                        }));
        return player;
    }
}
