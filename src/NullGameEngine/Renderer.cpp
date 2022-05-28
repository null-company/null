#include <queue>

#include <SFML/Graphics.hpp>

#include <Renderer.hpp>
#include <Utility.hpp>

namespace null {

    constexpr bool DEBUG_RENDER_B2_BODIES = false;

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
        while(!queue.empty()) {
            auto& srp = queue.top();
            window.draw(*(srp.sprite_ref));
            queue.pop();
        }
        // Toggle b2Body debug rendering by switching DEBUG_RENDER_B2_BODIES
        if constexpr (!DEBUG_RENDER_B2_BODIES)
            return;
        auto blist = scene.box2dWorld.GetBodyList();
        while (blist != nullptr) {
            auto const* list = blist->GetFixtureList();
            while (list != nullptr) {
                auto b2shape = (b2PolygonShape const*) list->GetShape();
                auto shape = sf::ConvexShape();
                shape.setFillColor(sf::Color::Transparent);
                shape.setOutlineColor(sf::Color::Red);
                shape.setOutlineThickness(-2);
                shape.setPointCount(b2shape->m_count);
                for (auto i = 0; i < b2shape->m_count; i++) {
                    shape.setPoint(i, Utility::meterToPixelVector<float>(b2shape->m_vertices[i]));
                }
                shape.setPosition(Utility::meterToPixelVector<float>(blist->GetPosition()));
                shape.setRotation(blist->GetAngle() * 180 / 3.1415926);
                window.draw(shape);
                list = list->GetNext();
            }
            blist = blist->GetNext();
        }
    }
}

