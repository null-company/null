
#include "Component.hpp"
#include "Weapons/WeaponAmmunition/BulletScript.hpp"
#include "GameObject.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"
#include <iostream>

namespace null {
    void null::BulletScript::start() {
        Component::start();
        gameObject.getSprite().setTexture(*ResourceManager::loadTexture("weapons/bullet.png"));
        gameObject.getSprite().setScale({0.010, 0.010});
        gameObject.makeDynamic();
        gameObject.getRigidBody()->SetBullet(true);
        gameObject.renderLayer = FOREGROUND3;
        gameObject.getRigidBody()->SetGravityScale(0);
        gameObject.getRigidBody()->SetLinearVelocity(
                {cos(2 * 3.14f / 360 * rotation) * speed, sin(2 * 3.14f / 360 * rotation) * speed});
        gameObject.setPosition(from);
        gameObject.visible = true;
    }

    void null::BulletScript::update() {
        Component::update();
        if (contactedGameObject) {
            std::cout << contactedGameObject->getName() << std::endl;
            gameObject.deleteMe();
            std::cout << gameObject.getParent().lock()->getChildren().size() << std::endl;
        }
    }

    BulletScript::BulletScript(GameObject& object, sf::Vector2f from, float rotation, float speed)
            : Component(object),
              from(from),
              rotation(rotation),
              speed(speed) {
    }
}

