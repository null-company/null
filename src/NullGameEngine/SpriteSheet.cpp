#include "SpriteSheet.hpp"

#include <utility>

namespace null {

    SpriteSheet::SpriteSheet(
            std::string  textureName,
            sf::Vector2i spriteSize,
            std::vector<Animation_s> animations
            ):  textureName(std::move(textureName)),
                spriteSize(spriteSize) {
        splitTextureIntoAnimations(animations);
        for (auto& anim : animations) {
            addAnimation(anim);
        }
    }

    void SpriteSheet::splitTextureIntoAnimations(std::vector<Animation_s> &animationVec) const {
        for (auto& anim : animationVec) {
            for(int i = anim.start; i <= anim.end; i++) {
                anim.framePositions.emplace_back(i * spriteSize.x, anim.row * spriteSize.y);
            }
        }
    }

    void SpriteSheet::addAnimation(const Animation_s& anim) {
        animations.emplace(anim.name, anim);
    }

    void SpriteSheet::setAnimation(const std::string& animName) {
        currAnimation = &animations.at(animName);
        currFrame = 0;
    }

    void SpriteSheet::setFrame(int frame) {
        currFrame = frame;
    }

    sf::IntRect SpriteSheet::getCurrentTextureRect() const {
        return {currAnimation->framePositions[currFrame], spriteSize};
    }
}
