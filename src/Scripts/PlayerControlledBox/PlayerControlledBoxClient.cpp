#include "PlayerControlledBox/PlayerControlledBoxClient.hpp"

#include <unordered_map>

#include <plog/Log.h>

#include <GameObject.hpp>
#include <ResourceManager.hpp>
#include <MainLoop.hpp>
#include <utils/NetMessageTransmitting.h>

namespace null {

    void PlayerControlledBoxClient::start() {
        // this is some experimental stuff:)
        auto boxTexture = ResourceManager::loadTexture("box.png");
        gameObject.getSprite().setTexture(*boxTexture);
        gameObject.getSprite().setScale(0.125f, 0.125f);
        gameObject.setPosition(200, 0);
        gameObject.renderLayer = FOREGROUND;
        gameObject.visible = true;
        gameObject.makeDynamic();
        gameObject.getRigidBody()->SetGravityScale(0.0f);

        messageQueue = &(MainLoop::clientNetworkManager->subscribe(gameObject.getGuid()));
    }

    namespace {
        enum Direction { Left, Up, Right, Down };
        Direction getDirectionByKey(const sf::Keyboard::Key& key) {
            static const std::unordered_map<sf::Keyboard::Key, Direction> keyToDirection {
                    { sf::Keyboard::Left, Left },
                    { sf::Keyboard::Up, Up },
                    { sf::Keyboard::Right, Right },
                    { sf::Keyboard::Down, Down },
            };
            return keyToDirection.at(key);
        }
        void moveRigidBody(b2Body* rigidBody, Direction direction) {
            constexpr float speedModule = 3.0f;
            b2Vec2 newVelocity = rigidBody->GetLinearVelocity();
            switch (direction) {
                case Left:
                    newVelocity.x = -speedModule;
                    break;
                case Up:
                    newVelocity.y = -speedModule;
                    break;
                case Right:
                    newVelocity.x = +speedModule;
                    break;
                case Down:
                    newVelocity.y = +speedModule;
                    break;
            }
            rigidBody->SetLinearVelocity(newVelocity);
        }
        void stopRigidBody(b2Body* rigidBody) {
            rigidBody->SetLinearVelocity({0.0f, 0.0f});
        }
    }

    namespace {
        net::GameMessage::ClientCommand makeMessage(uint64_t entityId, Direction direction) {
            net::GameMessage::ClientCommand command;
            command.set_subscriber_id(entityId);
            command.mutable_content()->add_uint32s(static_cast<uint32_t>(direction));
            return command;
        }

        void handleMessage(GameObject& gameObject, net::GameMessage::SubscriberState& newState) {
            // schema is uint32s: x, y
            float x = newState.mutable_content()->floats(0);
            float y = newState.mutable_content()->floats(1);
            gameObject.setPosition(x, y);
        }
    }

    void PlayerControlledBoxClient::update() {
        net::GameMessage::SubscriberState newState;
        bool newMessage = false;
//        LOGD << "Going over message que";
        if (!messageQueue->empty()) {
            for (auto m = messageQueue->front(); !messageQueue->empty(); m = messageQueue->front()) {
                //if (m.game_id() == id) {
                newState = m;
                newMessage = true;
                messageQueue->pop();
                if (messageQueue->empty()) {
                    break;
                }
                //}
            }
        }
//        LOGD << "Finished que";
        if (newMessage) {
            LOGD << "Message received";
            handleMessage(gameObject, newState);
        }
//        isMoving = false;
//        auto rigidBody = gameObject.getRigidBody();
        const auto keysToCheck = std::vector({
            sf::Keyboard::Left, sf::Keyboard::Up,
            sf::Keyboard::Right, sf::Keyboard::Down
        });

        for (const auto& key :  keysToCheck) {
            if (sf::Keyboard::isKeyPressed(key)) {
                auto direction = getDirectionByKey(key);
                MainLoop::clientNetworkManager->sendCommandToServer(
                        makeMessage(gameObject.getGuid(), direction)
                );
//                moveRigidBody(rigidBody, direction);
//                isMoving = true;
            }
        }

//        if (!isMoving) {
////            stopRigidBody(rigidBody);
//        }
    }
}
