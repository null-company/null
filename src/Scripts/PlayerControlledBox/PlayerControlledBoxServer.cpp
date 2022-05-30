#include "PlayerControlledBox/PlayerControlledBoxServer.hpp"
#include "serialized/serverConfig.pb.h"
#include "Serializer.hpp"

#include <SFML/System/Time.hpp>
#include <MainLoop.hpp>
#include <GameObject.hpp>
#include <ResourceManager.hpp>
#include <Network/NetworkManagerServerScript.hpp>

namespace null {

    void PlayerControlledBoxServer::start() {
        //  NOTE: server doesn't have to have graphical stuff, but it is still good
        // to keep it so that nothing breaks (too often)

        // this is some experimental stuff:)
//        auto boxTexture = ResourceManager::loadTexture("box.png");
//        gameObject.getSprite().setTexture(*boxTexture);
//        gameObject.getSprite().setScale(0.125f, 0.125f);
//        gameObject.setPosition(200, 0);
//        gameObject.renderLayer = FOREGROUND;
//        gameObject.visible = true;
//        gameObject.makeDynamic();
//        gameObject.getRigidBody()->SetGravityScale(0.0f);

        messageQueue.attachTo(
                &gameObject.getScene().lock()->findFirstByTag("network-manager")
                        .lock()->getScript<NetworkManagerServerScript>()->subscribe(gameObject.guid)
        );
        lastStateSnapshotTimer.restart();
    }

    namespace {
        enum Direction: uint32_t {
            Left, Up, Right, Down, Stop
        };

        void moveGameObject(GameObject& gameObject, Direction direction) {
            constexpr float delta = 3.0f;
            sf::Vector2f positionDelta;
            switch (direction) {
                case Left:
                    positionDelta.x = -delta;
                    break;
                case Up:
                    positionDelta.y = -delta;
                    break;
                case Right:
                    positionDelta.x = +delta;
                    break;
                case Down:
                    positionDelta.y = +delta;
                    break;
                case Stop:
                    break;
            }
            auto currentPosition = gameObject.getPosition();
            gameObject.setPosition(currentPosition + positionDelta);
        }
    }

    void PlayerControlledBoxServer::update() {
        messageQueue.processMessageIfAny([this](net::GameMessage::ClientCommand& commandMessage) {
            Direction direction;
            CommandConverter::restoreFromMessage(commandMessage.content(),
                                                 reinterpret_cast<uint32_t&>(direction));
            moveGameObject(gameObject, direction);
        });

        net::GameMessage::SubscriberState stateToBroadcast;
        stateToBroadcast.set_subscriber_id(gameObject.guid);

        const auto threshold = sf::milliseconds(10);
        if (lastStateSnapshotTimer.getElapsedTime() > threshold) {
            lastStateSnapshotTimer.restart();
            const auto& position = gameObject.getPosition();
            *stateToBroadcast.mutable_content() =
                    StateConverter::makeMessageFrom(position.x, position.y);
            MainLoop::serverArbiter->getGameServer().broadcastMessage(
                    stateToBroadcast
            );
        }
    }

    void PlayerControlledBoxServer::serialize(google::protobuf::Message& message) const {
        auto& msg = (serial::Script&)message;
        msg.mutable_player_controlled_box_server();
    }

    std::unique_ptr<Script> PlayerControlledBoxServer::deserialize(const google::protobuf::Message& message) {
        const auto& msg = (const serial::Script&)message;
        auto p_script = std::make_unique<PlayerControlledBoxServer>(
                *Serializer::currentDeserializationGameObject
                );
        return p_script;
    }
}
