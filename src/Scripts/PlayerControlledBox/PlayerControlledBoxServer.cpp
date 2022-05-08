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
        gameObject.getRigidBody()->SetGravityScale(0.0f);


        b2MassData massData;
        massData.mass = 5.0f;
        massData.center = gameObject.getRigidBody()->GetLocalCenter();
        gameObject.getRigidBody()->SetMassData(&massData);

        messageQueue = &(MainLoop::serverArbiter->getGameServer().subscribe(gameObject.getGuid()));
    }

    namespace {
        enum Direction {
            Left, Up, Right, Down, Stop
        };

        void moveGameObject(GameObject& gameObject, Direction direction) {
            constexpr float delta = 1.0f;
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

        net::GameMessage::SubscriberState makeMessage(uint64_t entityId, sf::Vector2f position) {
            net::GameMessage::SubscriberState state;
            state.set_subscriber_id(entityId);
            std::cout << "Sending x " << position.x << " y " << position.y << std::endl;
            state.mutable_content()->add_floats(position.x);
            state.mutable_content()->add_floats(position.y);
            return state;
        }
    }

    void PlayerControlledBoxServer::update() {
        if (!messageQueue->empty()) {
            auto lastCommandMessage = messageQueue->back();
            std::queue<net::GameMessage::ClientCommand> emptyQueue;
            std::swap(*messageQueue, emptyQueue);
            handleMessage(gameObject, lastCommandMessage);
        }

        MainLoop::serverArbiter->getGameServer().broadcastMessage(
                makeMessage(gameObject.getGuid(), gameObject.getPosition())
        );
    }
}
