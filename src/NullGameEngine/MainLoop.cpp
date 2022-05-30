#include <memory>

#include <MainLoop.hpp>
#include <Scene.hpp>
#include <Renderer.hpp>
#include <SpriteSheet.hpp>
#include <Utility.hpp>
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
            const bool continueMainLoop = sfmlWin == nullptr || sfmlWin->isOpen();
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
                            return false;
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
                        case sf::Event::MouseButtonPressed: {
                            if (e.mouseButton.button == sf::Mouse::Left) {
                                scene->windowMetaInfo.leftLock = true;
                            }
                            break;
                        }
                        case sf::Event::MouseButtonReleased: {
                            if (e.mouseButton.button == sf::Mouse::Left and scene->windowMetaInfo.leftLock) {
                                scene->windowMetaInfo.leftLock = false;
                                scene->windowMetaInfo.leftKeyPressed = true;
                            }
                            break;
                        }
                        case sf::Event::TextEntered: {
                            if (e.text.unicode <= 127) {
                                scene->windowMetaInfo.enteredChar = static_cast<char>(e.text.unicode);
                            }
                        }
                        default:
                            break;
                    }
                }
            }
            scene->update();
            scene->windowMetaInfo.resetKey();
            scene->windowMetaInfo.leftKeyPressed = false;
            scene->windowMetaInfo.enteredChar = '\0';
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
        }

        sceneStart:
        scene->getBox2dWorld().SetContactListener(new ContactListener());
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
