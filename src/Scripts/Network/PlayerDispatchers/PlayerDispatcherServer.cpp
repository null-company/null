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
        messageQueue.attachTo(
                &gameObject.getScene().lock()->findFirstByTag("network-manager")
                        .lock()->getScript<NetworkManagerServerScript>()->subscribe(gameObject.guid)
        );
    }

    void PlayerDispatcherServer::update() {
        messageQueue.processMessageIfAny([this](net::GameMessage::ClientCommand& commandMessage) {
            // we do not really care what the message is, it really only does
            uint32_t ignored;
            CommandConverter::restoreFromMessage(commandMessage.content(),
                                                 reinterpret_cast<uint32_t&>(ignored));
            ++currentPlayerIdx;
        });

        net::GameMessage::SubscriberState stateToBroadcast;
        stateToBroadcast.set_subscriber_id(gameObject.guid);

        *stateToBroadcast.mutable_content() =
                StateConverter::makeMessageFrom(players[currentPlayerIdx]);
        MainLoop::serverArbiter->getGameServer().broadcastMessage(
                stateToBroadcast
        );
    }

    PlayerDispatcherServer::PlayerDispatcherServer(GameObject& go) : Component(go) { }
}
