#include <memory>
#include <thread>

#include <MainLoop.hpp>
#include <Scene.hpp>
#include <Renderer.hpp>

namespace null {

    constexpr unsigned int MAX_FRAMERATE = 60;

    std::shared_ptr<Scene> MainLoop::scene = nullptr;

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
        // For now multithreading is disabled (because reasons)
        //sfmlWin.setActive(false);

        auto renderingThread = [](sf::RenderWindow &window) {
            window.setActive(true);
            while (window.isOpen()) {
                window.clear(sf::Color::Black);
                Renderer::render(window, *MainLoop::scene);
                window.display();
            }
        };

        sf::Thread rendererThread(renderingThread, std::ref(sfmlWin));
        //rendererThread.launch();

sceneStart:
        scene->start();

        try {
            while (sfmlWin.isOpen()) {
                scene->update();

                // todo dispatching user events such as keyboard inputs will probably take place here
                sf::Event e;
                while (sfmlWin.pollEvent(e)) {
                    switch (e.type) {
                        case sf::Event::EventType::Closed:
                            sfmlWin.close();
                            break;
                        default: break;
                    }
                }
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

