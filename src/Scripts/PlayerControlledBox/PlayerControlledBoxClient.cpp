#include "PlayerControlledBox/PlayerControlledBoxClient.hpp"

#include <unordered_map>

#include <GameObject.hpp>
#include <ResourceManager.hpp>
#include <MainLoop.hpp>
#include <utils/NetMessageTransmitting.h>
#include <Network/NetworkManagerClientScript.hpp>

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

        auto networkScript = gameObject.getScript<NetworkManagerClientScript>();
        messageQueue = &networkScript->subscribe(gameObject.getGuid());
    }

    namespace {
        enum Direction {
            Left, Up, Right, Down, Stop
        };

        Direction getDirectionByKey(const sf::Keyboard::Key& key) {
            static const std::unordered_map<sf::Keyboard::Key, Direction> keyToDirection{
                    {sf::Keyboard::Left,  Left},
                    {sf::Keyboard::Up,    Up},
                    {sf::Keyboard::Right, Right},
                    {sf::Keyboard::Down,  Down},
                    {sf::Keyboard::A,     Stop},
            };
            return keyToDirection.at(key);
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
            // schema is floats: x, y
            float x = newState.mutable_content()->floats(0);
            float y = newState.mutable_content()->floats(1);
            gameObject.setPosition(x, y);
        }
    }

    void PlayerControlledBoxClient::update() {

        if (!messageQueue->empty()) {
            auto lastStateMessage = messageQueue->back();
            std::queue<net::GameMessage::SubscriberState> emptyQueue;
            std::swap(*messageQueue, emptyQueue);
            handleMessage(gameObject, lastStateMessage);
        }

        const auto keysToCheck =
                std::vector({sf::Keyboard::Left, sf::Keyboard::Up,
                             sf::Keyboard::Right, sf::Keyboard::Down, sf::Keyboard::A});
        for (const auto& key: keysToCheck) {
            if (sf::Keyboard::isKeyPressed(key)) {
                auto direction = getDirectionByKey(key);
                MainLoop::clientNetworkManager->sendCommandToServer(
                        makeMessage(gameObject.getGuid(), direction)
                );
            }
        }
    }
}
