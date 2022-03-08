#include "ExampleCameraScript.hpp"
#include <algorithm>

namespace null {

    void ExampleCameraScript::start() {
        camera.view = sf::View(sf::FloatRect(0, 0, 1280, 720));
    }

    template<typename T>
    void move(sf::Rect<T>& rect, sf::Vector2<T> delta) {
        rect.left += delta.x;
        rect.top += delta.y;
    }

    sf::FloatRect ExampleCameraScript::computeNewViewRect() {
        // Write once read never (c) Artem
        auto trackedGOPosition = trackedObject->getSprite().getPosition();
        auto viewSize = camera.view.getSize();
        sf::FloatRect newViewRect(trackedGOPosition.x - viewSize.x / 2,
                           trackedGOPosition.y - viewSize.y / 2,
                                  viewSize.x,
                                  viewSize.y);

        auto rightTopPoint = sf::Vector2f(newViewRect.left + newViewRect.width, newViewRect.top);
        auto leftBottomPoint = sf::Vector2f(newViewRect.left, newViewRect.top + newViewRect.height);
        auto mapRect = map->getSprite().getGlobalBounds();
        auto rightTopPointMap = sf::Vector2f(mapRect.left + mapRect.width, mapRect.top);
        auto leftBottomPointMap = sf::Vector2f(mapRect.left, mapRect.top + mapRect.height);

        auto dx = std::max(rightTopPoint.x - rightTopPointMap.x, 0.0f);
        auto dy = std::min(rightTopPoint.y - rightTopPointMap.y, 0.0f);
        move(newViewRect, {-dx, -dy});
        dx = std::min(leftBottomPoint.x - leftBottomPointMap.x, 0.0f);
        dy = std::max(leftBottomPoint.y - leftBottomPointMap.y, 0.0f);
        move(newViewRect, {-dx, -dy});
        return newViewRect;
    }

    void ExampleCameraScript::update() {
        auto rect = computeNewViewRect();
        camera.view.setCenter(rect.left + rect.width / 2, rect.top + rect.height / 2);

    }

    ExampleCameraScript::ExampleCameraScript(Camera& camera) : CameraScript(camera) {}

    void ExampleCameraScript::setTrackedGameObject(GameObject& gameObject) {
        trackedObject = &gameObject;
    }

    void ExampleCameraScript::setMap(GameObject& newMap) {
        map = &newMap;
    }


}

