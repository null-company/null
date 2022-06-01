#include "Network/PlayerDispatchers/PlayerDispatcherClient.hpp"
#include <queue>
#include <network.h>
#include <memory>

#include <Script.hpp>
#include <GameObject.hpp>
#include <Scene.hpp>
#include <Network/NetworkManagerClientScript.hpp>
#include <client/ClientNetworkManager.h>
#include <Network/PrimitiveStateConverter.hpp>
#include <PlayerAnimation.hpp>

#include <plog/Log.h>

namespace null {


    void PlayerDispatcherClient::start() {
        LOGD << "CLIENT DISPATCHER START";
        auto networkManagerObject =
                gameObject.getScene().lock()->findFirstByTag("network-manager");
        networkManagerScript = networkManagerObject.lock()->getScript<NetworkManagerClientScript>();
        messageQueue.attachTo(&networkManagerScript->getNetworkManager().subscribe(gameObject.guid));
    }

    void PlayerDispatcherClient::update() {
        if (foundPlayer) {
            messageQueue.clear();
            return;
        }

        messageQueue.processMessageIfAny([this](net::GameMessage::SubscriberState& message) {
            std::string availablePlayer;
            PrimitiveStateConverter::restoreFromMessage(message.content(), availablePlayer);
            LOGD << "PLAYING AS " << availablePlayer;
            foundPlayer = true;
            foundPlayerName = availablePlayer;

            net::GameMessage::ClientCommand iReserveAPlayerMessage;
            iReserveAPlayerMessage.set_subscriber_id(gameObject.guid);
            *iReserveAPlayerMessage.mutable_content() =
                    CommandConverter::makeMessageFrom(42u); // any number
            networkManagerScript->getNetworkManager().sendCommandToServer(iReserveAPlayerMessage);
            // make player controllable
            auto& player = *gameObject.getSceneForce().findFirstByTag(availablePlayer).lock();
            auto playerAnimationP = player.getScript<PlayerAnimation>();
            playerAnimationP->controller = PlayerAnimation::Keyboard;
            for (const auto& callback : callbacks) {
                callback(availablePlayer);
            }
        });
    }

    PlayerDispatcherClient::PlayerDispatcherClient(GameObject& go) : Script(go) { }

    void PlayerDispatcherClient::registerPlayerFindingObserver(
            const std::function<void(const std::string&)>& callback
    ) {
        if (foundPlayer) {
            callback(foundPlayerName);
            return;
        }
        callbacks.push_back(callback);
    }
}
