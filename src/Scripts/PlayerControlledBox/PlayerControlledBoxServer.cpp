#include "PlayerControlledBox/PlayerControlledBoxServer.hpp"
#include "serialized/serverConfig.pb.h"

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
        gameObject.setPosition(200, 0);
//        gameObject.renderLayer = FOREGROUND;
//        gameObject.visible = true;
//        gameObject.makeDynamic();
//        gameObject.getRigidBody()->SetGravityScale(0.0f);

        messageQueue.attachTo(
                &gameObject.getScript<NetworkManagerServerScript>()->subscribe(gameObject.getGuid())
        );
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
            NetworkCommandManager::restoreStateFromMessage(commandMessage.content(),
                                                           reinterpret_cast<uint32_t&>(direction));
            moveGameObject(gameObject, direction);
        });

        net::GameMessage::SubscriberState stateToBroadcast;
        stateToBroadcast.set_subscriber_id(gameObject.getGuid());

        const auto& position = gameObject.getPosition();
        *stateToBroadcast.mutable_content() =
                NetworkStateManager::makeStateMessageFrom(position.x, position.y);
        MainLoop::serverArbiter->getGameServer().broadcastMessage(
                stateToBroadcast
        );
    }
}
