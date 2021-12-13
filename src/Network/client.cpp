#include <network.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include "utils/NetMessageTransmitting.h"

int main() {
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    Client client1("127.0.0.1", 5000);
    Client client2("127.0.0.1", 5000);
    Client client3("127.0.0.1", 5000);
    client1.createRoom();
    client2.connectRoom(client1.getRoom());
    client3.connectRoom(client1.getRoom());
    client1.sendChatMessage("Hello guys");
    client1.handleGameServerMessage(receiveNetMessage(client1.getGameServerSocket()).game_message());
    client2.handleGameServerMessage(receiveNetMessage(client2.getGameServerSocket()).game_message());
    client3.handleGameServerMessage(receiveNetMessage(client3.getGameServerSocket()).game_message());
}