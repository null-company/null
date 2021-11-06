#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <Renderer.hpp>

namespace null {

    void Renderer::render(const sf::RenderWindow& window, const Scene& scene) {
        auto gameObjects = scene.gameObjects;
        for (GameObject& go : gameObjects) {
            auto sprite = *go.getSprite().lock();
            window.draw(sprite);
        }
    }

}

