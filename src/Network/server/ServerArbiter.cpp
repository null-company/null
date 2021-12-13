#include "server/ServerArbiter.h"
#include <plog/Log.h>

#include <utility>
#include "plog/Initializers/RollingFileInitializer.h"
#include "utils/util.h"
#include "utils/NetMessageTransmitting.h"

//TODO: what about destruction the game?
std::string ServerArbiter::createNewGameSimulation() {
    PLOGD << "New game simulation is creating";
    net::GameServerConfig gameServerConfig;
    gameServers.emplace_back(std::make_unique<GameServer>());
    gameServers.back()->listen(sf::IpAddress(this->getIP()), freePorts.back());
    freePorts.pop_back();
    gameServers.back()->launch();

    std::string roomCode = generateSixLetterCode();
    while (roomCodeToServerNum.contains(roomCode)) {
        roomCode = generateSixLetterCode();
    }

    PLOGD << "Room code was chosen: " << roomCode;
    roomCodeToServerNum[roomCode] = gameServers.size() - 1;
    return roomCode;
}

ServerArbiter::ServerArbiter(std::function<void()> simulationThread) : NetClientCollector(
        std::move(simulationThread)) {}

ServerArbiter::ServerArbiter() : NetClientCollector(),
                                 freePorts({6000, 6001, 6002, 6003, 6004, 6005, 6006}),
                                 gameServers() {
}


void ServerArbiter::sendGameServerConfig(sf::TcpSocket &client, const std::string &roomCode) {
    net::NetMessage message;
    net::GameServerConfig *serverConfig = message.mutable_server_config();
    if (!roomCodeToServerNum.contains(roomCode)) {
        sendNetMessage(client, message);
    }
    GameServer &server = *(gameServers[roomCodeToServerNum[roomCode]]);
    serverConfig->set_room_code(roomCode);
    serverConfig->set_v4(server.getIP());
    serverConfig->set_server_port(server.getPort());
    sendNetMessage(client, message);
}

void ServerArbiter::handleNetMessage(sf::TcpSocket &client, const net::NetMessage &message) {
    switch (message.body_case()) {
        case net::NetMessage::kGenerateRoomRequest: {
            LOGD << "Request about asking generating new game received";
            std::string roomCode = createNewGameSimulation();
            sendRoomCode(client, roomCode);
            break;
        }
        case net::NetMessage::kConnectRoom: {
            LOGD << "Request about asking getting game server config by server";
            sendGameServerConfig(client, message.connect_room().room_code());
            break;
        }
        case net::NetMessage::kGameMessage:
        case net::NetMessage::kClientInfo:

        case net::NetMessage::kServerConfig:
        case net::NetMessage::BODY_NOT_SET:
            break;
    }
}

void ServerArbiter::sendRoomCode(sf::TcpSocket &socket, const std::string &roomCode) {
    LOGD << "sending room code to the " << socket.getRemoteAddress() << " " << socket.getRemotePort();
    net::NetMessage netMessage;
    netMessage.mutable_connect_room()->set_room_code(roomCode);
    sendNetMessage(socket, netMessage);
}

