#include "Network/NetworkManagerServerScript.hpp"
#include "exceptions/NetworkException.h"

#include <unordered_map>

#include <GameObject.hpp>
#include <MainLoop.hpp>
#include <utils/NetMessageTransmitting.h>

namespace null {

    void NetworkManagerServerScript::start() {
        LOGD << "Server script start";
        gameServer = &null::MainLoop::serverArbiter->getGameServer();
    }


    void NetworkManagerServerScript::update() {
        int readyClientIdx = gameServer->getFirstReadySocketIdx();
        if (readyClientIdx == -2) {
            return;
        }
        if (readyClientIdx == -1) {
            gameServer->acceptNewClient();
            return;
        }
        try {
            sf::TcpSocket& client = gameServer->getClient(readyClientIdx);
            net::NetMessage message = null::Network::Utils::receiveNetMessage(client);
            gameServer->handleNetMessage(readyClientIdx, message);
        } catch (const ReceiveException& exception) {
            auto status = exception.getStatus();
            if (status == sf::Socket::Disconnected) {
                gameServer->disconnectClient(readyClientIdx);
                return;
            }
            throw ReceiveException("Unexpected client receive exception status",
                                   exception.getStatus());
        }
    }

    std::queue<net::GameMessage::ClientCommand>& NetworkManagerServerScript::subscribe(uint64_t guid) {
        return gameServer->subscribe(guid);
    }

    NetworkManagerServerScript::NetworkManagerServerScript(GameObject& go) : Script(go) { }

    void NetworkManagerServerScript::serialize(google::protobuf::Message& message) const {
        Component::serialize(message);
    }

    std::unique_ptr<Script> NetworkManagerServerScript::deserialize(const google::protobuf::Message& message) {
        return std::unique_ptr<Script>();
    }
}
