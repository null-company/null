#include "Animation.hpp"

#include <GameObject.hpp>
#include <ResourceManager.hpp>
#include <iostream>

namespace null {
    Animation::Animation(GameObject& gameObject, SpriteSheet& spriteSheet):
        Component(gameObject), spriteSheet(spriteSheet) {
        auto& sprite = gameObject.getSprite();
        sprite.setTexture(*ResourceManager::loadTexture(spriteSheet.textureName));
        sprite.setTextureRect(sf::IntRect(sf::Vector2i() , spriteSheet.spriteSize));
    }

    void Animation::update() {
        gameObject.getSprite().setTextureRect(spriteSheet.getCurrentTextureRect());
    }


}
