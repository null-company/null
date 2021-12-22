#include <ButtonScript.hpp>
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
        std::cout << "calling callback" << std::endl;
        callback();
    }

    ButtonScript::ButtonScript(GameObject& gameObject, sf::Texture unpressedButtonTexture,
            sf::Texture pressedButtonTexture, const std::function<void()> callback) :
        Script(gameObject), unpressedButtonTexture(unpressedButtonTexture),
        pressedButtonTexture(pressedButtonTexture), callback(callback) { };

}

