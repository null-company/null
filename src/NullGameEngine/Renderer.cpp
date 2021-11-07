#include <memory>

#include <SFML/Graphics.hpp>

#include <Renderer.hpp>

namespace null {

    void Renderer::render(sf::RenderWindow& window, const Scene& scene) {
        for (const std::unique_ptr<GameObject>& go : scene.gameObjects) {
            // todo this is ugly
            sf::Sprite sprite = *(*go).getSprite().lock();
            if (go->getIsVisible()) {
                window.draw(sprite);
            }
        }
    }

}

