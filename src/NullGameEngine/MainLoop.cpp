#include <memory>

#include <MainLoop.hpp>
#include <Scene.hpp>
#include <Renderer.hpp>
#include <SpriteSheet.hpp>
#include "exceptions/NetworkException.h"

namespace null {

    ServerArbiter* MainLoop::serverArbiter = nullptr;
    thread_local GameServer* MainLoop::gameServer = nullptr;
    thread_local ClientNetworkManager* MainLoop::clientNetworkManager = nullptr;
    thread_local bool MainLoop::isServer = false;

    std::shared_ptr<Scene> MainLoop::scene = nullptr;

    namespace {
        constexpr unsigned int MAX_FRAMERATE = 60;
        void clientNetworkJobStep(ClientNetworkManager& clientNetworkManager) {
            try {
                while (true) {

                    auto message = receiveNetMessage(
                            clientNetworkManager.getClient().getGameServerSocket()).game_message();
                    clientNetworkManager.distributeMessageToSubscribers(*message.mutable_subscriber_state());
                }
            } catch (const ReceiveException& noMessagesLeft) { }
        }
        void serverNetworkJobStep(GameServer& gameServer) {
            int readyClientIdx = gameServer.getFirstReadySocketIdx();
            if (readyClientIdx == -2) {
                return;
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
                throw ReceiveException("Unexpected client receive exception status",
                                       exception.getStatus());
            }
        }
        /**
         * @return false iff simulation is over
         */
        bool simulationStep(Scene* scene,
                            sf::RenderWindow* sfmlWin,
                            ServerArbiter* serverArbiter,
                            ClientNetworkManager* clientNetworkManager,
                            bool isServer) {
            bool continueMainLoop = sfmlWin == nullptr || sfmlWin->isOpen();
            if (!continueMainLoop) {
                return false;
            }
            sf::Event e{};
            if (!isServer) {
                if (sfmlWin == nullptr) {
                    throw std::logic_error("no window exists for client");
                }
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
            if (isServer) {
                serverNetworkJobStep(serverArbiter->getGameServer());
            } else {
                clientNetworkJobStep(*clientNetworkManager);
            }
            scene->update();

            if (!isServer) {
                sfmlWin->clear(sf::Color::Black);
                Renderer::render(*sfmlWin, *scene);
                sfmlWin->display();
            }
            return true;
        }
    }

    int MainLoop::run() {
        sf::RenderWindow* sfmlWin = nullptr;
        if (!isServer) {
            sfmlWin = new sf::RenderWindow(sf::VideoMode(1280, 720), "{[Null]}");
            sfmlWin->setFramerateLimit(MAX_FRAMERATE);
            sfmlWin->setMouseCursorVisible(false);
        }

        sceneStart:
        scene->start();
        try {
            while (true) {
                if (!simulationStep(scene.get(),
                                    sfmlWin,
                                    serverArbiter,
                                    clientNetworkManager,
                                    isServer)) {
                    break;
                }
            }
        } catch (const SceneChangedException& sceneChanged) {
            goto sceneStart;
        }
        return 0;
    }
}
