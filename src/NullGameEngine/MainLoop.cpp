#include <memory>

#include <MainLoop.hpp>
#include <Scene.hpp>
#include <Renderer.hpp>
#include <SpriteSheet.hpp>

namespace null {

    ServerArbiter* MainLoop::serverArbiter = nullptr;
    GameServer* MainLoop::gameServer = nullptr;
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
        return run(true);
    }

    int MainLoop::run(bool noHead) {
        sf::RenderWindow* sfmlWin = nullptr;
        if (!noHead) {
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
                if (!noHead) {
                    while (sfmlWin->pollEvent(e)) {
                        switch (e.type) {
                            case sf::Event::EventType::Closed:
                                sfmlWin->close();
                                break;
                            case sf::Event::Resized:{
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }

                scene->update();
                if (!noHead) {
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
