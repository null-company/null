#include <SFML/System.hpp>

#include <CursorAnimation.hpp>
#include <GameObject.hpp>
#include <iostream>
#include <Scene.hpp>
#include <ButtonScript.hpp>
#include <MainLoop.hpp>
#include "Serializer.hpp"

namespace null {
    void CursorAnimation::start() {
        spriteSheet.setAnimation(cursorAnim);
        this->windowMetaInfo = &(gameObject.getScene().lock()->getWindowMetaInfo());
    }

    void CursorAnimation::update() {
        auto coords = windowMetaInfo->absoluteMouseWorldCoords;
        gameObject.setPosition(coords);
        if (frameCount++ == 3) {
            spriteSheet.setFrame((spriteSheet.currFrame + 1) % spriteSheet.currAnimation->end);
            frameCount = 0;
        }
        Animation::update();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto rigidBody = gameObject.getRigidBody();
            if (rigidBody == nullptr) {
                return;
            }
            b2ContactEdge* collision = nullptr;
            if (rigidBody->GetContactList() == nullptr) {
                return;
            }
            for (auto contactEdge = rigidBody->GetContactList();
                 contactEdge != nullptr;
                 contactEdge = contactEdge->next) {
                if (contactEdge->contact->IsTouching() == true) {
                    collision = contactEdge;
                    break;
                }
            }
            if (collision == nullptr) {
                return;
            }
            GameObject* otherObject = reinterpret_cast<GameObject*>(collision->other->GetUserData().pointer);
            if (otherObject != nullptr) {
                auto& scripts = otherObject->getScripts();
                if (scripts.size() > 0) {
                    auto buttonScript = otherObject->getScript<ButtonScript>();
                    if (buttonScript != nullptr) {
                        buttonScript->press();
                    }
                }
            }
        }
    }

    CursorAnimation::CursorAnimation(GameObject& gameObject, SpriteSheet spriteSheet) : Animation(gameObject,
                                                                                                  spriteSheet) {}

    void CursorAnimation::serialize(google::protobuf::Message& message) const {
        auto& msg = dynamic_cast<serial::Script&>(message);
        auto s_anim = msg.mutable_cursor_animation();
        auto s_ss = s_anim->mutable_animation()->mutable_spritesheet();
        spriteSheet.serialize(*s_ss);
    }

    std::unique_ptr<Component> CursorAnimation::deserialize(const google::protobuf::Message& message) {
        auto& msg = dynamic_cast<const serial::Script&>(message);
        auto s_anim = msg.cursor_animation();
        auto s_ss = s_anim.animation().spritesheet();
        auto p_ss = SpriteSheet::deserialize(s_ss);
        return std::make_unique<CursorAnimation>(
                *Serializer::currentDeserializationGameObject,
                *p_ss
                );
    }
}
