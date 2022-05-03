#include <gtest/gtest.h>
#include <server/ServerArbiter.h>
#include "client/ClientNetworkManager.h"
#include "exceptions/NetworkException.h"

//
//TEST(NetworkClientManager, createTest) {
//    srand(time(nullptr));
//    int port = rand() % 23 + 5000;
//    ServerArbiter serverArbiter;
//    port++;
//    serverArbiter.listen("127.0.0.1", port);
//    serverArbiter.launch();
//    ClientNetworkManager client1("127.0.0.1", port);
//
//    client1.getClient().createRoom();
//    std::string roomCode = client1.getClient().getRoomCode();
//    LOGD << "room code is: " << roomCode;
//}
//
//TEST(NetworkClientManager, connectTest) {
//    ServerArbiter serverArbiter;
//    int port = rand() % 23 + 5000;
//    serverArbiter.listen("127.0.0.1", port);
//    serverArbiter.launch();
//    ClientNetworkManager client1("127.0.0.1", port);
//    ClientNetworkManager client2("127.0.0.1", port);
//    ClientNetworkManager client3("127.0.0.1", port);
//    ClientNetworkManager client4("127.0.0.1", port);
//
//    client1.getClient().createRoom();
//
//    std::string roomCode = client1.getClient().getRoomCode();
//    client2.getClient().connectRoom(roomCode);
//    client3.getClient().connectRoom(roomCode);
//    client4.getClient().connectRoom(roomCode);
//    LOGD << "room code is: " << roomCode;
//}

TEST(NetworkClientManager, subtest) {
    using namespace std::chrono_literals;

    ServerArbiter serverArbiter;
    int port = rand() % 23 + 5000;
    serverArbiter.listen("127.0.0.1", port);
    serverArbiter.launch();
    ClientNetworkManager client1("127.0.0.1", port);
    ClientNetworkManager client2("127.0.0.1", port);
    ClientNetworkManager client3("127.0.0.1", port);
    ClientNetworkManager client4("127.0.0.1", port);

    client1.getClient().createRoom();

    std::string roomCode = client1.getClient().getRoomCode();
    client2.getClient().connectRoom(roomCode);
    client3.getClient().connectRoom(roomCode);
    client4.getClient().connectRoom(roomCode);

    auto &q1 = client1.subscribe(1, net::GameMessage::kChatMessage);
    client1.subscribe(1, net::GameMessage::kPlayerInfo);

    auto &q2 = client1.subscribe(2, net::GameMessage::kChatMessage);
    auto &q3 = client3.subscribe(1, net::GameMessage::kChatMessage);
    client3.subscribe(1, net::GameMessage::kBoxInfo);
    client3.subscribe(1, net::GameMessage::kPlayerInfo);
    auto &q4 = client3.subscribe(5, net::GameMessage::kPlayerInfo);
    client3.subscribe(5, net::GameMessage::kBoxInfo);

    client2.getClient().sendChatMessage("Meow");
    net::NetMessage netMessage;
    netMessage.mutable_game_message()->mutable_boxinfo()->set_meow(2);
    sendNetMessage(client2.getClient().getGameServerSocket(), netMessage);
    netMessage.mutable_game_message()->mutable_player_info()->set_x(4);
    sendNetMessage(client2.getClient().getGameServerSocket(), netMessage);
    client2.getClient();

    std::this_thread::sleep_for(1s);

    try {
        while (true) {
            auto message = client1.receiveMessage();
            client1.distributeMessageToSubscribers(message);
        }
    } catch (ReceiveException &e) {

    }
    try {
        while (true) {
            auto message = client3.receiveMessage();
            client3.distributeMessageToSubscribers(message);
        }
    } catch (ReceiveException &e) {

    }
    ASSERT_EQ(q1.size(), 2);
    ASSERT_EQ(q2.size(), 1);
    ASSERT_EQ(q3.size(), 3);
    ASSERT_EQ(q4.size(), 2);
}