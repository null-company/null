#include "CursorAnimation.hpp"
#include <GameObject.hpp>
#include <iostream>

namespace null {
    void CursorAnimation::start() {
        setAnimation(cursorAnim);
    }

    void CursorAnimation::update() {
        gameObject.getSprite().setPosition(sf::Vector2f(sf::Mouse::getPosition() * 2 - sf::Vector2i(640, 380)));
        if (frameCount++ == 3) {
            setFrame((currFrame + 1) % currAnimation->end);
            frameCount = 0;
        }
        Animation::update();
    }

    CursorAnimation::CursorAnimation(GameObject& gameObject, SpriteSheet spriteSheet) : Animation(gameObject, spriteSheet) { }
}