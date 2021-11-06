#include <SFML/Graphics.hpp>

#include <Renderer.hpp>

namespace null {

    void Renderer::render(sf::RenderWindow& window, const Scene& scene) {
        auto gameObjects = scene.gameObjects;
        for (GameObject& go : gameObjects) {
            sf::Sprite sprite = *go.getSprite().lock();
            window.draw(sprite);
        }
    }

}

