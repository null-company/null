#include "PlayerControlledBox/PlayerControlledBoxClient.hpp"
#include "Serializer.hpp"

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

    void PlayerControlledBoxClient::serialize(google::protobuf::Message& message) const {
        auto& msg = (serial::Script&)message;
        auto s_script = msg.mutable_player_controlled_box_client();
        s_script->set_nw_manager_script_guid(networkManagerScript->guid);
    }

    std::unique_ptr<Script> PlayerControlledBoxClient::deserialize(const google::protobuf::Message& message) {
        const auto& msg = (const serial::Script&)message;
        auto s_script = msg.player_controlled_box_client();
        auto p_script = std::make_unique<PlayerControlledBoxClient>(
                (*Serializer::currentDeserializationGameObject)
                );
        Serializer::addToBeSetPointer((Entity**)&p_script->networkManagerScript, s_script.nw_manager_script_guid());
        return p_script;
    }
}
