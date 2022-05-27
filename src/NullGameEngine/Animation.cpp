#include "Animation.hpp"
#include <Serializer.hpp>

#include <GameObject.hpp>
#include <ResourceManager.hpp>

namespace null {
    Animation::Animation(GameObject& gameObject, SpriteSheet const& spriteSheet):
        Component(gameObject), spriteSheet(spriteSheet) {
        auto& sprite = gameObject.getSprite();
        sprite.setTexture(*ResourceManager::loadTexture(spriteSheet.textureName));
        sprite.setTextureRect(sf::IntRect(sf::Vector2i() , spriteSheet.spriteSize));
    }

    void Animation::update() {
        gameObject.getSprite().setTextureRect(spriteSheet.getCurrentTextureRect());
    }

    void Animation::serialize(google::protobuf::Message& message) const {
        auto msg = dynamic_cast<serial::Script&>(message);
        auto s_anim = msg.mutable_animation();
        auto s_spritesheet = s_anim->mutable_spritesheet();
        spriteSheet.serialize(*s_spritesheet);
    }

    std::unique_ptr<Component> Animation::deserialize(const google::protobuf::Message& message) {
        auto msg = dynamic_cast<const serial::Script&>(message);
        auto const& s_anim = msg.animation();
        auto const& s_spritesheet = s_anim.spritesheet();
        auto p_spritesheet = SpriteSheet::deserialize(s_spritesheet);
        return std::make_unique<Animation>(
                *Serializer::currentDeserializationGameObject,
                *p_spritesheet
                );
    }
}
