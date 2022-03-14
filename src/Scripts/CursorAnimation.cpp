#include "CursorAnimation.hpp"
#include <GameObject.hpp>
#include <iostream>
#include <Scene.hpp>

namespace null {
    void CursorAnimation::start() {
        spriteSheet.setAnimation(cursorAnim);
        this->windowMetaInfo = &(gameObject.getScene().lock()->getWindowMetaInfo());
    }

    void CursorAnimation::update() {
        auto coords = windowMetaInfo->relativeWindowMouseCoord;
        gameObject.getSprite().setPosition(coords);
        if (frameCount++ == 3) {
            spriteSheet.setFrame((spriteSheet.currFrame + 1) % spriteSheet.currAnimation->end);
            frameCount = 0;
        }
        Animation::update();
    }

    CursorAnimation::CursorAnimation(GameObject& gameObject, SpriteSheet spriteSheet) : Animation(gameObject,
                                                                                                  spriteSheet) {}
}