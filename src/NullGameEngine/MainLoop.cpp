#include <memory>

#include <MainLoop.hpp>
#include <Scene.hpp>
#include <Renderer.hpp>
#include <SpriteSheet.hpp>
#include "exceptions/NetworkException.h"

namespace null {

    ServerArbiter* MainLoop::serverArbiter = nullptr;
//    GameServer* MainLoop::gameServer = nullptr;
    ClientNetworkManager* MainLoop::clientNetworkManager = nullptr;

    constexpr unsigned int MAX_FRAMERATE = 60;

    std::shared_ptr<Scene> MainLoop::scene = nullptr;

    /**
     * There potentially can be network initialization
     * ###############################################
     * ClientNetworkManager MainLoop::clientNetworkManager = ClientNetworkManager("127.0.0.1", 5000);
     * ServerArbiter MainLoop::serverArbiter = ServerArbiter();
     * ###############################################
    **/
    bool MainLoop::isServer = false;
//    sf::Window* MainLoop::window = nullptr;

//    sf::Window& MainLoop::getWindow() {
//        return *window;
//    }

    int MainLoop::run() {
        sf::RenderWindow* sfmlWin = nullptr;
        if (!isServer) {
            sfmlWin = new sf::RenderWindow(sf::VideoMode(1280, 720), "{[Null]}");
            // window = &sfmlWin;
            sfmlWin->setFramerateLimit(MAX_FRAMERATE);
            sfmlWin->setMouseCursorVisible(false);
        }

        sceneStart:
        scene->start();
        try {
            bool cont = sfmlWin == nullptr || sfmlWin->isOpen();
            while (cont) {


                // todo dispatching user events such as keyboard inputs will probably take place here
                sf::Event e{};
                if (!isServer) {
                    while (sfmlWin->pollEvent(e)) {
                        switch (e.type) {
                            case sf::Event::EventType::Closed:
                                sfmlWin->close();
                                break;
                            case sf::Event::Resized: {
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
                if (!isServer) {
                    try { // client
                        while (true) {

                            auto message = receiveNetMessage(
                                    clientNetworkManager->getClient().getGameServerSocket()).game_message();
                            clientNetworkManager->distributeMessageToSubscribers(*message.mutable_subscriber_state());
                        }
                    } catch (ReceiveException e) {

                    }
                } else {
                    auto gameServerStep = []() {
                        auto& gameServer  = serverArbiter->getGameServer();
                        int readyClientIdx = gameServer.getFirstReadySocketIdx();
                        if (readyClientIdx == -2) {
                            return ;
                        }
                        if (readyClientIdx == -1) {
                            gameServer.acceptNewClient();
                            return;
                        }
                        try {
                            sf::TcpSocket& client = gameServer.getClient(readyClientIdx);
                            net::NetMessage message = receiveNetMessage(client);
                            gameServer.handleNetMessage(readyClientIdx, message);
                        } catch (const ReceiveException& exception) {
                            auto status = exception.getStatus();
                            if (status == sf::Socket::Disconnected) {
                                gameServer.disconnectClient(readyClientIdx);
                                return;
                            }
                            std::cout << status << std::endl;
                            throw ReceiveException("Unexpected client receive exception status",
                                                   exception.getStatus());
                        }
                    };
                    gameServerStep();
                }
                scene->update();

                if (!isServer) {
                    sfmlWin->clear(sf::Color::Black);
                    Renderer::render(*sfmlWin, *MainLoop::scene);
                    sfmlWin->display();
                }
            }
        }
        catch (const SceneChangedException& sceneChanged) {
            goto sceneStart;
        }

        return 0;
    }

}
