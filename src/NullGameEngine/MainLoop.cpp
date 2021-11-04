#include <MainLoop.hpp>
#include <Scene.hpp>

namespace null {

    // todo this is a dummy implementation, copied from the earlier draft
    int MainLoop::run() {
sceneStart:
        scene->start();
        // todo following code should be a part of the scene
        sf::RenderWindow sfmlWin(sf::VideoMode(1280, 720), "{[Null]}");

        sf::Texture nullTexture;
        if (!nullTexture.loadFromFile("../null.jpg")) {
          return EXIT_FAILURE;
        }
        sf::Sprite nullPicture(nullTexture);


        try {
            while (sfmlWin.isOpen()) {
                scene->update();
                // todo the following code should be a part of the scene

                sf::Event e;
                while (sfmlWin.pollEvent(e)) {

                    switch (e.type) {
                        case sf::Event::EventType::Closed:
                            sfmlWin.close();
                            break;
                        default: break;
                    }
                }

                sfmlWin.clear();
                sfmlWin.draw(nullPicture);
                sfmlWin.display();
            }
        } 
        catch (const SceneChangedException& sceneChanged) {
            goto sceneStart;
        }

        return 0;
    }

}

