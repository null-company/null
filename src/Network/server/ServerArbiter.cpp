#include "server/ServerArbiter.h"
#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"
#include "utils/util.h"
#include "utils/NetMessageTransmitting.h"

//TODO: reuse previous game slots
std::string ServerArbiter::createNewGameSimulation() {
    PLOGD << "New game simulation is creating";
    net::GameServerConfig gameServerConfig;
    gameServers.emplace_back(std::make_unique<GameServer>(ipAddress, freePorts.front()));
    gameServers.back()->launch();
    freePorts.pop_front();

    std::string roomCode = generateSixLetterCode();
    while (roomCodeToServerNum.contains(roomCode)) {
        roomCode = generateSixLetterCode();
    }

    PLOGD << "Room code was chosen: " << roomCode;
    roomCodeToServerNum[roomCode] = gameServers.size() - 1;
    return roomCode;
}

ServerArbiter::ServerArbiter(sf::IpAddress ipAddress, uint16_t port) : NetClientCollector(ipAddress, port),
                                                                       freePorts(),
                                                                       gameServers() {
    for (uint16_t i = 5000u; i < 5030u; i++) {
        freePorts.push_back(i);
    }
    LOGD << "Game server arbiter was initialized";
}

std::string ServerArbiter::handleRoomCodeMessage(const net::ConnectRoom &room) {
    LOGD << "Room code was received";
    const std::string &roomCode = room.room_code();
    return roomCode;
}

void ServerArbiter::sendGameServerConfig(sf::TcpSocket &client, const std::string &roomCode) {
    net::NetMessage message;
    net::GameServerConfig *serverConfig = message.mutable_server_config();
    if (!roomCodeToServerNum.contains(roomCode)) {
        sendNetMessage(client, message);
    }
    const GameServer &server = *(gameServers[roomCodeToServerNum[roomCode]]);
    serverConfig->set_room_code(roomCode);
    serverConfig->set_v4(server.ipAddress.toInteger());
    serverConfig->set_server_port(port);
    sendNetMessage(client, message);
}

void ServerArbiter::handleNetMessage(sf::TcpSocket &client, const net::NetMessage &message) {
    if (message.has_generate_room_request()) {
        LOGD << "Request about asking generating new game received";
        std::string roomCode = createNewGameSimulation();
        sendRoomCode(client, roomCode);
    } else if (message.has_connect_room()) {
        LOGD << "Request about asking getting game server config by server";
        sendGameServerConfig(client, message.connect_room().room_code());
    }
}

void ServerArbiter::sendRoomCode(sf::TcpSocket &socket, const std::string &roomCode) {
    LOGD << "sending room code to the " << socket.getRemoteAddress() << " " << socket.getRemotePort();
    net::NetMessage netMessage;
    netMessage.mutable_connect_room()->set_room_code(roomCode);
    sendNetMessage(socket, netMessage);
}

