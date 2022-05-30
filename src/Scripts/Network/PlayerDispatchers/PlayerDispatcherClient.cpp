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

#include <plog/Log.h>

namespace null {


    void PlayerDispatcherClient::start() {
        std::cout << "CLIENT DISPATCHER START" << std::endl;
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
//            LOGD << "PLAYING AS " << availablePlayer;
            std::cout << "PLAYING AS " << availablePlayer << std::endl;
            foundPlayer = true;

            net::GameMessage::ClientCommand iReserverAPlayerMessage;
            iReserverAPlayerMessage.set_subscriber_id(gameObject.guid);
            *iReserverAPlayerMessage.mutable_content() =
                    CommandConverter::makeMessageFrom(42u); // any number
            networkManagerScript->getNetworkManager().sendCommandToServer(iReserverAPlayerMessage);
            // make player controllable
//            throw std::runtime_error("todo");
        });
    }

    PlayerDispatcherClient::PlayerDispatcherClient(GameObject& go) : Script(go) { }
}
