#include <ButtonScript.hpp>
#include <Script.hpp>
#include <GameObject.hpp>
#include <Scene.hpp>
#include <iostream>

namespace null {

    void ButtonScript::start() {
        sprite = &gameObject.getSprite();

        sprite->setTexture(unpressedButtonTexture);
        gameObject.makeStatic(gameObject.getScene().lock()->getBox2dWorld());
        rigidBody = gameObject.getRigidBody();
        std::cout << rigidBody->GetPosition().x << " " << rigidBody->GetPosition().y << std::endl;
        rigidBody->GetFixtureList()->SetSensor(true);
    }

    void ButtonScript::update() {
        // only the head of the list is checked under the assumtion that
        // this script is used for menus
        auto contactList = rigidBody->GetContactList();
        bool cursorIsHovered = false;
        if (contactList != nullptr) {
            cursorIsHovered = contactList->contact->IsTouching();
        }

        sprite->setTexture(cursorIsHovered ? pressedButtonTexture : unpressedButtonTexture);
    }

    void ButtonScript::press() {
        callback();
    }

    ButtonScript::ButtonScript(GameObject& gameObject, sf::Texture unpressedButtonTexture,
                               sf::Texture pressedButtonTexture, const std::function<void()> callback) :
            Script(gameObject), unpressedButtonTexture(unpressedButtonTexture),
            pressedButtonTexture(pressedButtonTexture), callback(callback) {};

}

