//
// Created by artemonchelus on 11.05.22.
//

#include "GameObjectSensor.hpp"
#include "DeleteScript.hpp"

#include <utility>
#include <functional>

namespace null {
    void GameObjectSensor::start() {
        Component::start();
        timer.start();
        gameObject.makeStatic();
        gameObject.getRigidBody()->GetFixtureList()->SetSensor(true);
    }

    void GameObjectSensor::update() {
        Component::update();
        gameObject.setPosition(gameObject.getParent().lock()->getPosition() + bias);
        if (timer.expired()) {
            gameObject.destroy();
        }
    }

    GameObjectSensor::GameObjectSensor(GameObject& object,
                                       const sf::Vector2f& bias,
                                       std::shared_ptr<GameObject> objectToAdd,
                                       std::function<void(GameObject* touchedGameObject,
                                                          GameObject* sensor)> touchHandler,
                                       const Timer& timer) :
            Component(object),
            timer(timer),
            touchHandler(std::move(touchHandler)),
            bias(bias),
            objectToAdd(std::move(objectToAdd)) {
        gameObject.getSprite().setTexture(*this->objectToAdd->getSprite().getTexture());
        gameObject.getSprite().scale(this->objectToAdd->getSprite().getScale());
        gameObject.renderLayer = this->objectToAdd->renderLayer;
        gameObject.visible = this->objectToAdd->visible;
    }

    void GameObjectSensor::onTouch(null::GameObject* touchedObject) {
        this->touchHandler(touchedObject, &this->gameObject);
    }

    std::shared_ptr<GameObject> GameObjectSensor::getObjectToAdd() const {
        return objectToAdd;
    }

    void onPlayerEnter(GameObject* touchedObject, GameObject* sensorObject) {
        if (touchedObject->getScript<PlayerAnimation>() && sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
            auto weaponStorage = touchedObject->findFirstChildrenByTag("WeaponStorage");
            if(! weaponStorage){
                return;
            }
            auto script = weaponStorage->getScript<WeaponStorage>();
            script->addWeapon(std::move(sensorObject->getScript<GameObjectSensor>()->getObjectToAdd()));
            sensorObject->getScript<DeleteScript>()->deleteGameObject();
        }
    }
}
