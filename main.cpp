#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow sfmlWin(sf::VideoMode(1280, 720), "{[Null]}");

    sf::Texture nullTexture;
    if (!nullTexture.loadFromFile("../null.jpg"))
    {
      return EXIT_FAILURE;
    }
    sf::Sprite nullPicture(nullTexture);

    while (sfmlWin.isOpen()) {

        sf::Event e;
        while (sfmlWin.pollEvent(e)) {

            switch (e.type) {
            case sf::Event::EventType::Closed:
                sfmlWin.close();
                break;
            }
        }

        sfmlWin.clear();
        sfmlWin.draw(nullPicture);
        sfmlWin.display();
    }
    return 0;
}
