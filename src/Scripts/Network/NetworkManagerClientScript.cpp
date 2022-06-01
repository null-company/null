#include "Network/NetworkManagerClientScript.hpp"
#include "exceptions/NetworkException.h"

#include <GameObject.hpp>
#include <MainLoop.hpp>
#include <utils/NetMessageTransmitting.h>
#include <SceneLoader.hpp>

namespace null {

    void NetworkManagerClientScript::start() {
        LOGD << "Client server script start";
        try {
            networkManager = std::make_unique<ClientNetworkManager>(serverArbiterIp, serverArbiterPort);
        } catch (const NetworkException& e) {
            std::cerr << "Tried to connect to server but failed" << std::endl;
            exit(20);
        }

        if (SceneLoader::getContext() == nullptr) {
            networkManager->getClient().createRoomAndConnect();
            LOGD << "Created room. Code: " << networkManager->getClient().getRoomCode();
            std::cout << "Created room. Code: " << networkManager->getClient().getRoomCode()<< std::endl;
        } else {
            const auto* roomCode = static_cast<const std::string*>(SceneLoader::getContext());
            LOGD << "Joined room";
            networkManager->getClient().connectRoom(*roomCode);
        }
    }


    void NetworkManagerClientScript::update() {
        try {
            while (true) {
                auto message = null::Network::Utils::receiveNetMessage(
                        networkManager->getClient().getGameServerSocket()).game_message();
                networkManager->distributeMessageToSubscribers(*message.mutable_subscriber_state());
            }
        } catch (const ReceiveException& noMessagesLeft) {}
    }

    NetworkManagerClientScript::NetworkManagerClientScript(GameObject& go) : Script(go) {}

    ClientNetworkManager& NetworkManagerClientScript::getNetworkManager() const {
        return *networkManager;
    }
}
