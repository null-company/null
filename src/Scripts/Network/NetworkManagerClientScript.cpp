#include "Network/NetworkManagerClientScript.hpp"
#include "exceptions/NetworkException.h"

#include <GameObject.hpp>
#include <MainLoop.hpp>
#include <utils/NetMessageTransmitting.h>

namespace null {

    void NetworkManagerClientScript::start() {
        LOGD << "Client server script start";
        try {
            networkManager = std::make_unique<ClientNetworkManager>(ipToConnectTo, port);
        } catch (const NetworkException& e) {
            std::cerr << "Tried to connect to server but failed" << std::endl;
            exit(20);
        }

        dialog:
        std::cout << "Create room? [y/n]" << std::flush;
        std::string answer;
        std::cin >> answer;
        std::cout << "\n";
        if (answer == "y") {
            networkManager->getClient().createRoomAndConnect();
            std::cout << "Room code: " << networkManager->getClient().getRoomCode() << std::endl;
            LOGD << "Created room. Code: " << networkManager->getClient().getRoomCode();
        } else if (answer == "n") {
            std::cout << "Room code:" << std::flush;
            std::string roomCodeToConnect;
            std::cin >> roomCodeToConnect;
            std::cout << std::endl;
            networkManager->getClient().connectRoom(roomCodeToConnect);
        } else {
            std::cerr << "Wrong input. Try again" << std::endl;
            goto dialog;
        }
    }


    void NetworkManagerClientScript::update() {
        try {
            while (true) {
                auto message = null::Network::Utils::receiveNetMessage(
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
