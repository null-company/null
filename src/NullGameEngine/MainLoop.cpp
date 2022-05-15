#include <memory>

#include <MainLoop.hpp>
#include <Scene.hpp>
#include <Renderer.hpp>
#include <SpriteSheet.hpp>
#include "exceptions/NetworkException.h"
#include <Utility.hpp>
#include <NetworkPlayerScript.hpp>
#include <exceptions/NetworkException.h>
#include <iostream>
#include "Physics/ContactListener.hpp"

namespace null {

    ServerArbiter* MainLoop::serverArbiter = nullptr;
    thread_local bool MainLoop::attachWindow = false;

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
                        case sf::Event::KeyPressed: {
                            scene->windowMetaInfo.keyEvent = e.key;
                            break;
                        }
                        case sf::Event::KeyReleased: {
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
            scene->update();
            scene->windowMetaInfo.resetKey();
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
        if (!attachWindow) {
            sfmlWin = new sf::RenderWindow(sf::VideoMode(1280, 720), "{[Null]}");
            sfmlWin->setFramerateLimit(MAX_FRAMERATE);
            sfmlWin->setMouseCursorVisible(false);
            sf::RenderWindow sfmlWin(sf::VideoMode(1280, 720), "{[Null]}");
            sfmlWin.setFramerateLimit(MAX_FRAMERATE);
            sfmlWin.setMouseCursorVisible(false);
//            std::unordered_set<uint> gg;
        }

        sceneStart:
        if(!attachWindow){
            scene->getBox2dWorld().SetContactListener(new ContactListener());
        }
        scene->start();
        try {
            while (true) {
                if (!simulationStep(scene.get(), sfmlWin, attachWindow)) {
                    break;
                }
            }
        } catch (const SceneChangedException& sceneChanged) {
            goto sceneStart;
        }
        return 0;
    }
}
