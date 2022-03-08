
#pragma once

#include "GameObject.hpp"
#include <memory>
#include <box2d/box2d.h>
#include <box2d/b2_math.h>
namespace null {
    class MapManager {
    private:
        b2World& box2dWorld;
    public:
        MapManager(b2World& box2dWorld) : box2dWorld(box2dWorld) {}

        std::shared_ptr<GameObject> makeBorder(float x, float y, float width, float height);

        std::shared_ptr<GameObject> makeBorder(float x, float y, float width, float height, float bsize=30);

        std::shared_ptr<GameObject> makeBorder(sf::Sprite& sprite, float bsize=30);
    };
}



