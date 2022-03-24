#include "Weapon/StraightWeaponScript.hpp"
#include "Script.hpp"
#include "GameObject.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"
#include "box2d/box2d.h"

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
        gameObject.renderLayer = serial::FOREGROUND2;
        gameObject.visible = true;
//        gameObject.makeDynamic(gameObject.getScene().lock()->getBox2dWorld());

    }

    void StraightWeaponScript::update() {
        Component::update();
        auto parent = gameObject.getParent().lock();
        if (parent == nullptr) {
            return;
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
        if(weapondirection.x < 0){
            delta = 180;
        }
        gameObject.getSprite().setRotation(delta + atan(weapondirection.y / weapondirection.x) * 180 / 3.141592);
    }

    void StraightWeaponScript::shoot(sf::Vector2f from, sf::Vector2f to) {

    }

    StraightWeaponScript::StraightWeaponScript(GameObject& object) : WeaponScript(object) {}
}
