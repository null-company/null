#include <memory>

#include <MainLoop.hpp>
#include <Scene.hpp>
#include <Renderer.hpp>
#include <SpriteSheet.hpp>
#include "exceptions/NetworkException.h"

namespace null {

    ServerArbiter* MainLoop::serverArbiter = nullptr;
//    thread_local GameServer* MainLoop::gameServer = nullptr;
//    thread_local ClientNetworkManager* MainLoop::clientNetworkManager = nullptr;
    thread_local bool MainLoop::isServer = false;
    bool MainLoop::isNetworkingEnabled = false;

    std::shared_ptr<Scene> MainLoop::scene = nullptr;

    namespace {
        constexpr unsigned int MAX_FRAMERATE = 60;
        /**
         * @return false iff simulation is over
         */
        bool simulationStep(Scene* scene,
                            sf::RenderWindow* sfmlWin,
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
                if (!simulationStep(scene.get(), sfmlWin, isServer)) {
                    break;
                }
            }
        } catch (const SceneChangedException& sceneChanged) {
            goto sceneStart;
        }
        return 0;
    }
}
