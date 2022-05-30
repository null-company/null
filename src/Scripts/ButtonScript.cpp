#include "ButtonScript.hpp"

#include <ResourceManager.hpp>
#include <Script.hpp>
#include <GameObject.hpp>
#include <Scene.hpp>

namespace null {

    void ButtonScript::start() {
        sprite = &gameObject.getSprite();

        sprite->setTexture(unpressedButtonTexture);
        gameObject.makeStatic(gameObject.getScene().lock()->getBox2dWorld());
        rigidBody = gameObject.getRigidBody();
        rigidBody->GetFixtureList()->SetSensor(true);

        onPressSound = &ResourceManager::getSound("menu-choose-option.ogg");
        onHoverSound = &ResourceManager::getSound("hover-mouse.ogg");
    }

    void ButtonScript::update() {
        // only the head of the list is checked under the assumption that
        // this script is used for menus
        auto contactList = rigidBody->GetContactList();
        bool cursorIsHovered = false;
        if (contactList != nullptr) {
            cursorIsHovered = contactList->contact->IsTouching();
        }

        if (!cursorIsHovered) {
            playedHover = false;
        }

        sprite->setTexture(cursorIsHovered ? pressedButtonTexture : unpressedButtonTexture);
        if (cursorIsHovered && !playedHover) {
            onHoverSound->play();
            playedHover = true;
        }
    }

    void ButtonScript::press() {
        onPressSound->play();
        callback();
    }

    ButtonScript::ButtonScript(GameObject& gameObject, sf::Texture unpressedButtonTexture,
                               sf::Texture pressedButtonTexture, const std::function<void()> callback) :
            Script(gameObject), unpressedButtonTexture(unpressedButtonTexture),
            pressedButtonTexture(pressedButtonTexture), callback(callback) {}

}
