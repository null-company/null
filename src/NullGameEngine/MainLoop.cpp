#include <memory>
#include <thread>

#include <MainLoop.hpp>
#include <Scene.hpp>
#include <Renderer.hpp>
#include <SpriteSheet.hpp>
#include <Utility.hpp>
#include <NetworkPlayerScript.hpp>
#include <exceptions/NetworkException.h>
#include <iostream>

namespace null {

    constexpr unsigned int MAX_FRAMERATE = 60;

    std::shared_ptr<Scene> MainLoop::scene = nullptr;

    /**
     * There potentially can be network initialization
     * ###############################################
     * ClientNetworkManager MainLoop::clientNetworkManager = ClientNetworkManager("127.0.0.1", 5000);
     * ###############################################
    **/
    sf::Window* MainLoop::window = nullptr;

    sf::Window& MainLoop::getWindow() {
        return *window;
    }

    // todo this is a dummy implementation, copied from the earlier draft
    int MainLoop::run() {
        sf::RenderWindow sfmlWin(sf::VideoMode(1280, 720), "{[Null]}");
        window = &sfmlWin;
        sfmlWin.setFramerateLimit(MAX_FRAMERATE);
        sfmlWin.setMouseCursorVisible(false);

sceneStart:
        scene->start();

        try {
            while (sfmlWin.isOpen()) {


                // todo dispatching user events such as keyboard inputs will probably take place here
                sf::Event e;
                while (sfmlWin.pollEvent(e)) {
                    switch (e.type) {
                        case sf::Event::EventType::Closed:
                            sfmlWin.close();
                            break;
                        case sf::Event::Resized:{
                            break;
                        }
                        default:
                            break;
                    }
                }

                scene->update();
                sfmlWin.clear(sf::Color::Black);
                Renderer::render(sfmlWin, *MainLoop::scene);
                sfmlWin.display();
            }
        }
        catch (const SceneChangedException& sceneChanged) {
            goto sceneStart;
        }

        return 0;
    }

}

