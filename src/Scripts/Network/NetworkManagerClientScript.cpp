#include "Network/NetworkManagerClientScript.hpp"
#include "exceptions/NetworkException.h"

#include <GameObject.hpp>
#include <MainLoop.hpp>
#include <utils/NetMessageTransmitting.h>

namespace null {

    void NetworkManagerClientScript::start() {
        LOGD << "Client server script start";
        networkManager = std::make_unique<ClientNetworkManager>(ipToConnectTo, port);
        networkManager->getClient().createRoom();
        LOGD << "Created room. Code: " << networkManager->getClient().getRoomCode();
    }


    void NetworkManagerClientScript::update() {
        try {
            while (true) {
                auto message = receiveNetMessage(
                        networkManager->getClient().getGameServerSocket()).game_message();
                networkManager->distributeMessageToSubscribers(*message.mutable_subscriber_state());
            }
        } catch (const ReceiveException& noMessagesLeft) { }
    }

    NetworkManagerClientScript::NetworkManagerClientScript(GameObject& go) : Script(go) { }

    ClientNetworkManager& NetworkManagerClientScript::getNetworkManager() const {
        return *networkManager;
    }
}
