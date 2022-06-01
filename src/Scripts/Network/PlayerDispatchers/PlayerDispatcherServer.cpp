#include "Network/PlayerDispatchers/PlayerDispatcherServer.hpp"
#include "Network/PrimitiveStateConverter.hpp"

#include <queue>
#include <network.h>
#include <memory>

#include <Script.hpp>
#include <NullGameEngine.hpp>
#include <GameObject.hpp>
#include <Scene.hpp>
#include <client/ClientNetworkManager.h>
#include <MainLoop.hpp>

namespace null {

    void PlayerDispatcherServer::start() {
        LOGD << "SERVER DISPATCHER START";
        if (players.empty()) {
            throw std::runtime_error("No players were provided");
        }
        messageQueue.attachTo(
                &gameObject.getScene().lock()->findFirstByTag("network-manager")
                        .lock()->getScript<NetworkManagerServerScript>()->subscribe(gameObject.guid)
        );
    }

    void PlayerDispatcherServer::update() {
        if (isFinished) {
            // clean the queue
            messageQueue.clear();
            return;
        }
        messageQueue.processMessageIfAny([this](net::GameMessage::ClientCommand& commandMessage) {
            // we do not really care what the message is. Receiving it only means we are doing good
            uint32_t ignored;
            CommandConverter::restoreFromMessage(commandMessage.content(), ignored);
            LOGD << "gave away player " << players[currentPlayerIdx];
            ++currentPlayerIdx;
            if (currentPlayerIdx >= players.size()) {
                LOGD << "no players left";
                isFinished = true;
            }
        });

        if (currentPlayerIdx >= players.size()) {
            return;
        }

        net::GameMessage::SubscriberState availablePlayerTag;
        availablePlayerTag.set_subscriber_id(gameObject.guid);

        *availablePlayerTag.mutable_content() =
                StateConverter::makeMessageFrom(players[currentPlayerIdx]);
        MainLoop::serverArbiter->getGameServer().broadcastMessage(
                availablePlayerTag
        );
    }

    PlayerDispatcherServer::PlayerDispatcherServer(GameObject& go) : Script(go) { }
}
