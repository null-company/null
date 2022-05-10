#include "PlayerControlledBox/PlayerControlledBoxServer.hpp"
#include "serialized/serverConfig.pb.h"

#include <unordered_map>

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

        messageQueue = &gameObject.getScript<NetworkManagerServerScript>()->subscribe(gameObject.getGuid());
    }

    namespace {
        enum Direction {
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

        void handleMessage(GameObject& gameObject, net::GameMessage::ClientCommand& command) {
            auto direction =
                    Direction(command.mutable_content()->uint32s(0));
            moveGameObject(gameObject, direction);
        }
    }

    void PlayerControlledBoxServer::update() {
        if (!messageQueue->empty()) {
            auto lastCommandMessage = messageQueue->back();
            std::queue<net::GameMessage::ClientCommand> emptyQueue;
            std::swap(*messageQueue, emptyQueue);
            handleMessage(gameObject, lastCommandMessage);
        }

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
