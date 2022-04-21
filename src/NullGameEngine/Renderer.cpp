#include <memory>
#include <queue>

#include <SFML/Graphics.hpp>

#include <Renderer.hpp>

namespace null {

    void Renderer::render(sf::RenderWindow& window, const Scene& scene) {
        if (scene.camera->getScript<CameraScript>()) {
            window.setView(scene.camera->getScript<CameraScript>()->view);
        }
        scene.windowMetaInfo.windowsSize = window.getSize();
        scene.windowMetaInfo.absoluteMouseWorldCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        using SpriteRefPair = struct SpriteRefPair_s {
            RenderLayer renderLayer;
            sf::Sprite* sprite_ref;
        };

        auto compSRP = [](SpriteRefPair x, SpriteRefPair y) {
            return x.renderLayer > y.renderLayer;
        };

        auto queue = std::priority_queue<SpriteRefPair, std::vector<SpriteRefPair>, decltype(compSRP)>(compSRP);
        scene.sceneTreeForEachDo([&queue](GameObject& go) -> void {
            if (go.visible) {
                auto spriteRefPair = SpriteRefPair{go.renderLayer, &(go.getSprite())};
                queue.push(spriteRefPair);
            }
        });

        for (auto& srp = (SpriteRefPair&) queue.top(); !queue.empty(); srp = queue.top()) {
            window.draw(*(srp.sprite_ref));
            queue.pop();
            if (queue.empty()) {
                return;
            }
        }
    }
}

