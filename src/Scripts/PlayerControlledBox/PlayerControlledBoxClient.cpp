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
        gameObject.renderLayer = serial::FOREGROUND;
        gameObject.visible = true;
        gameObject.makeDynamic();
        gameObject.getRigidBody()->SetGravityScale(0.0f);

        auto networkManagerObject = gameObject.getScene().lock()->findFirstByTag("network-manager");
        networkManagerScript = networkManagerObject.lock()->getScript<NetworkManagerClientScript>();
        messageQueue.attachTo(&networkManagerScript->getNetworkManager().subscribe(gameObject.guid));
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

    void PlayerControlledBoxClient::update() {
        messageQueue.processMessageIfAny([this](net::GameMessage::SubscriberState& message) {
            networkStateManager.restoreFromMessage(message.content());
            gameObject.setPosition(x, y);
        });

        const auto keysToCheck =
                std::vector({sf::Keyboard::Left, sf::Keyboard::Up,
                             sf::Keyboard::Right, sf::Keyboard::Down, sf::Keyboard::A});
        for (const auto& key: keysToCheck) {
            if (sf::Keyboard::isKeyPressed(key)) {
                auto direction = getDirectionByKey(key);
                net::GameMessage::ClientCommand commandMessage;
                commandMessage.set_subscriber_id(gameObject.guid);
                *commandMessage.mutable_content() =
                        CommandConverter::makeMessageFrom(static_cast<uint32_t>(direction));
                networkManagerScript->getNetworkManager().sendCommandToServer(commandMessage);
            }
        }
    }

    PlayerControlledBoxClient::PlayerControlledBoxClient(GameObject& go)
            : Script(go) { }
}
