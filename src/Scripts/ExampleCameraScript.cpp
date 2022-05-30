#include "ExampleCameraScript.hpp"
#include "Scene.hpp"
#include "plog/Log.h"
#include <algorithm>
#include <Serializer.hpp>

namespace null {

    void ExampleCameraScript::start() {
        CameraScript::start();
    }

    template<typename T>
    void move(sf::Rect<T>& rect, sf::Vector2<T> delta) {
        rect.left += delta.x;
        rect.top += delta.y;
    }

    sf::FloatRect ExampleCameraScript::computeNewViewRect() {
        // Write once read never (c) Artem
        auto trackedGOPosition = trackedObject->getSprite().getPosition();
        auto viewSize = sf::Vector2f(windowMetaInfo->windowsSize);
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
        view.setCenter(rect.left + rect.width / 2, rect.top + rect.height / 2);
        view.setSize(rect.width, rect.height);
    }

    ExampleCameraScript::ExampleCameraScript(GameObject& camera) : CameraScript(camera) {

    }

    void ExampleCameraScript::setTrackedGameObject(GameObject& gameObject) {
        trackedObject = &gameObject;
    }

    void ExampleCameraScript::setMap(GameObject& newMap) {
        map = &newMap;
    }

    void ExampleCameraScript::serialize(google::protobuf::Message& message) const {
        auto& msg = dynamic_cast<serial::Script&>(message);
        auto s_script = msg.mutable_example_camera_script();
        s_script->set_tracked_object_guid(trackedObject->guid);
        s_script->set_map_guid(map->guid);
    }

    std::unique_ptr<Component> ExampleCameraScript::deserialize(const google::protobuf::Message& message) {
        auto& msg = dynamic_cast<const serial::Script&>(message);
        auto const& s_script = msg.example_camera_script();
        auto p_script = std::make_unique<ExampleCameraScript>(
                *Serializer::currentDeserializationGameObject
                );
        // This is an example of getting the pointers to serialized entities
        // (they're set in Serializer::getSceneFromFile)
        Serializer::addToBeSetPointer(
                (Entity**)(&p_script->map),
                s_script.map_guid()
                );

        Serializer::addToBeSetPointer(
                (Entity**)(&p_script->trackedObject),
                s_script.tracked_object_guid()
                );

        return p_script;
    }

}

