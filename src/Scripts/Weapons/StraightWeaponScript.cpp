#include "Weapon/StraightWeaponScript.hpp"
#include "Script.hpp"
#include "GameObject.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"
#include "box2d/box2d.h"
#include "Weapon/BulletScript.hpp"

namespace null {
    void StraightWeaponScript::start() {
        Component::start();
        auto parent = gameObject.getParent().lock();
        if (parent == nullptr) {
            return;
        }
        sf::Texture* weaponTexture = ResourceManager::loadTexture("weapon.png");
        gameObject.getSprite().setOrigin(sf::Vector2f(250, 150));
        gameObject.getSprite().setTexture(*weaponTexture);
        gameObject.getSprite().scale(0.24, 0.24);
        gameObject.renderLayer = FOREGROUND2;
        gameObject.visible = true;
//        gameObject.makeDynamic(gameObject.getScene().lock()->getBox2dWorld());

    }

    void StraightWeaponScript::update() {
        Component::update();
        auto scene = gameObject.getScene().lock();
        if (scene == nullptr) {
            return;
        }

        auto parent = gameObject.getParent().lock();
        if (parent == nullptr) {
            return;
        }
        auto& winInfo = gameObject.getScene().lock()->getWindowMetaInfo();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and checkIfCanShoot()) {
            shoot(gameObject.getPosition(), scene->getWindowMetaInfo().absoluteMouseWorldCoords);
        }
        auto coords = parent->getPosition() + sf::Vector2f(70, 70);
        gameObject.setPosition(coords);
        setWeaponRotation();
    }

    void StraightWeaponScript::setWeaponRotation() {
        auto scene = gameObject.getScene().lock();
        if (scene == nullptr) {
            return;
        }
        auto window_info = scene->getWindowMetaInfo().absoluteMouseWorldCoords;
        sf::Vector2f weapondirection = window_info - gameObject.getPosition();
        double delta = 0;
        if (weapondirection.x < 0) {
            delta = 180;
        }
        gameObject.getSprite().setRotation(delta + atan(weapondirection.y / weapondirection.x) * 180 / 3.141592);
    }

    void StraightWeaponScript::shoot(sf::Vector2f from, sf::Vector2f to) {
        saveShotInfo();
        auto bullet = std::make_shared<GameObject>();
        bullet->addScript<BulletScript>(*bullet, from, gameObject.getSprite().getRotation() + d(gen), speed);
        gameObject.addChild(std::move(bullet));


    }

    StraightWeaponScript::StraightWeaponScript(GameObject& object, double deviance) : WeaponScript(object) {
        d = std::normal_distribution<>(0, deviance);
    }
}
