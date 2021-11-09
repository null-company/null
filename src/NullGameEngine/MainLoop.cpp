#include <memory>

#include <MainLoop.hpp>
#include <Scene.hpp>
#include <Renderer.hpp>

namespace null {

    std::unique_ptr<Scene> MainLoop::scene = nullptr;

    // todo this is a dummy implementation, copied from the earlier draft
    int MainLoop::run() {
        sf::RenderWindow sfmlWin(sf::VideoMode(1280, 720), "{[Null]}");

        sfmlWin.setActive(false);

        auto renderingThread = [](sf::RenderWindow &window) {
            window.setActive(true);
            while (window.isOpen()) {
                window.clear(sf::Color::Black);
                Renderer::render(window, *MainLoop::scene);
                window.display();
            }
        };

        sf::Thread rendererThread(renderingThread, std::ref(sfmlWin));
        rendererThread.launch();

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

            }
        } 
        catch (const SceneChangedException& sceneChanged) {
            goto sceneStart;
        }

        return 0;
    }

}

