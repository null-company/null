#include "PlayerControlledBox/PlayerControlledBoxServer.hpp"
#include "serialized/serverConfig.pb.h"

#include <unordered_map>

#include <MainLoop.hpp>
#include <GameObject.hpp>
#include <ResourceManager.hpp>

namespace null {

    void PlayerControlledBoxServer::start() {
        // this is some experimental stuff:)
        auto boxTexture = ResourceManager::loadTexture("box.png");
        gameObject.getSprite().setTexture(*boxTexture);
        gameObject.getSprite().setScale(0.125f, 0.125f);
        gameObject.setPosition(200, 0);
        gameObject.renderLayer = FOREGROUND;
        gameObject.visible = true;
        gameObject.makeDynamic();

        messageQueue = &(MainLoop::gameServer->subscribe(gameObject.getGuid()));
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
        void handleMessage(GameObject& gameObject, net::GameMessage::ClientCommand& command) {
            auto rigidBody = gameObject.getRigidBody();
            auto direction =
                    Direction(command.mutable_content()->uint32s(0));
            moveRigidBody(rigidBody, direction);
        }
        net::GameMessage::SubscriberState makeMessage(uint64_t entityId, sf::Vector2f position) {
            net::GameMessage::SubscriberState state;
            state.set_subscriber_id(entityId);
            state.mutable_content()->add_floats(position.x);
            state.mutable_content()->add_floats(position.y);
            return state;
        }
    }

    void PlayerControlledBoxServer::update() {
        net::GameMessage::ClientCommand command;
        bool newMessage = false;
        for (auto m = messageQueue->front(); !messageQueue->empty(); m = messageQueue->front(), messageQueue->pop()) {
            //if (m.game_id() == id) {
            command = m;
            newMessage = true;
            //}
        }
        if (newMessage) {
            handleMessage(gameObject, command);
        }
        MainLoop::gameServer->broadcastMessage(
                makeMessage(gameObject.getGuid(), gameObject.getPosition())
        );
//        isMoving = false;
//        auto rigidBody = gameObject.getRigidBody();
//        auto keysToCheck = std::vector({
//                                               sf::Keyboard::Left, sf::Keyboard::Up,
//                                               sf::Keyboard::Right, sf::Keyboard::Down
//                                       });

//        for (const auto& key :  keysToCheck) {
//            if (sf::Keyboard::isKeyPressed(key)) {
//                auto direction = getDirectionByKey(key);
//                moveRigidBody(rigidBody, direction);
//                isMoving = true;
//            }
//        }

//        if (!isMoving) {
//            stopRigidBody(rigidBody);
//        }
    }
}
