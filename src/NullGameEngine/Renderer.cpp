#include <memory>
#include <queue>

#include <SFML/Graphics.hpp>

#include <Renderer.hpp>

namespace null {

    void Renderer::render(sf::RenderWindow& window, const Scene& scene) {
        typedef struct {
            const RenderLayer& renderLayer;
            const sf::Sprite& sprite_ref;
        } SpriteRefPair;

        auto compSRP = [](SpriteRefPair* x, SpriteRefPair* y) {
            return x->renderLayer > y->renderLayer;
        };

        auto gamer = std::priority_queue<SpriteRefPair*, std::vector<SpriteRefPair*>, decltype(compSRP)>(compSRP);
        for (const auto& go : scene.gameObjects) {
            if (go->getIsVisible()) {
                auto spriteRefPair = new SpriteRefPair {go->getRenderLayer(), go->getSprite()};
                gamer.push(spriteRefPair);
            }
        }
        for (auto srp = gamer.top(); !gamer.empty(); srp = gamer.top()) {
            window.draw(srp->sprite_ref);
            gamer.pop();
        }

    }

}

