#pragma once

#include <memory>

#include <box2d/box2d.h>
#include <box2d/b2_math.h>
#include <GameObject.hpp>

namespace null {
    class MapManager {
    private:
        b2World& box2dWorld;
    public:
        explicit MapManager(b2World& box2dWorld) : box2dWorld(box2dWorld) {}


        std::shared_ptr<GameObject> makeBorder(float x, float y, float width, float height, float bsize=30);

        std::shared_ptr<GameObject> makeBorder(sf::Sprite& sprite, float bsize=30);
    };
}



