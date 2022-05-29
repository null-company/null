#include "SpriteSheet.hpp"
#include "scripts.pb.h"
#include "internals.pb.h"

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

    void SpriteSheet::serialize(google::protobuf::Message& message) const {
        auto s_ss = dynamic_cast<serial::SpriteSheet&>(message);
        s_ss.mutable_frame_size()->set_x(this->spriteSize.x);
        s_ss.mutable_frame_size()->set_y(this->spriteSize.y);
        s_ss.set_image_name(this->textureName);
        for (auto const& [a, b]: this->animations) {
            auto t = s_ss.add_anim();
            t->set_name(a);
            t->set_row(b.row);
            t->set_start(b.start);
            t->set_end(b.end);
        }
        message.CopyFrom(s_ss);
    }

    std::unique_ptr<SpriteSheet> SpriteSheet::deserialize(const google::protobuf::Message& message) {
        auto s_ss = dynamic_cast<const serial::SpriteSheet&>(message);
        auto anim_vec = std::vector<Animation_s>();
        for (auto const& i : s_ss.anim()) {
            Animation_s anim;
            anim.name = i.name();
            anim.row = i.row();
            anim.start = i.start();
            anim.end = i.end();
            anim_vec.push_back(anim);
        }
        auto ss = SpriteSheet(
                s_ss.image_name(),
                {s_ss.frame_size().x(), s_ss.frame_size().y()},
                anim_vec
                );
        auto p_ss = std::make_unique<SpriteSheet>(ss);
        return p_ss;
    }
}
