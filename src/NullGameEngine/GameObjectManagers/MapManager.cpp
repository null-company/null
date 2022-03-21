//
// Created by artemonchelus on 09.03.2022.
//

#include "MapManager/MapManager.hpp"
#include "GameObject.hpp"
#include "Utility.hpp"
#include <box2d/box2d.h>
#include <box2d/b2_math.h>
#include <ResourceManager.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace null {

    std::shared_ptr<GameObject>
    null::MapManager::makeBorder(float x, float y, float width, float height, float bsize) {

        auto borderShapes = std::vector<sf::IntRect>();
        borderShapes.emplace_back(0, 0, int(width + 2 * bsize), int(bsize));
        borderShapes.emplace_back(0, 0, int(bsize), int(height + 2 * bsize));
        borderShapes.emplace_back(0, 0, int(width + 2 * bsize), int(bsize));
        borderShapes.emplace_back(0, 0, int(bsize), int(height + 2 * bsize));

        auto pos = std::vector<sf::Vector2f>();
        pos.emplace_back(x - bsize, y - bsize);
        pos.emplace_back(x + width, y - bsize);
        pos.emplace_back(x - bsize, y + height);
        pos.emplace_back(x - bsize, y - bsize);
        auto walls = std::make_shared<GameObject>();
        for (int i = 0; i < pos.size(); i++) {
            auto wall = std::make_shared<GameObject>();
            wall->getSprite().setTexture(*ResourceManager::loadTexture("pivo.png"));
            wall->getSprite().setTextureRect(borderShapes[i]);
            wall->setPosition(pos[i]);
            wall->makeStatic(box2dWorld);
            wall->renderLayer = FOREGROUND1;
            wall->visible = true;
            walls->addChild(std::move(wall));
        }
        return walls;

    }

    std::shared_ptr<GameObject>
    null::MapManager::makeBorder(sf::Sprite& sprite, float bsize) {
        auto x = sprite.getPosition().x;
        auto y = sprite.getPosition().y;
        auto width = sprite.getGlobalBounds().width;
        auto height = sprite.getGlobalBounds().height;
        return makeBorder(x, y, width, height, bsize);
    }
}
