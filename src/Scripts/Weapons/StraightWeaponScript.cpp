#include "Weapon/StraightWeaponScript.hpp"
#include "Script.hpp"
#include "GameObject.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"
#include "box2d/box2d.h"
#include "Weapon/BulletScript.hpp"
#include "Serializer.hpp"

namespace null {
    void StraightWeaponScript::start() {
        WeaponScript::start();
        gameObject.getSprite().setOrigin(sf::Vector2f(250, 150));
    }

    void StraightWeaponScript::update() {
        WeaponScript::update();
        auto scene = gameObject.getScene().lock();
        if (scene == nullptr) {
            return;
        }

        auto parent = gameObject.getParent().lock();
        if (parent == nullptr) {
            return;
        }
        auto& winInfo = gameObject.getScene().lock()->getWindowMetaInfo();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && checkIfCanShoot()) {
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

    void StraightWeaponScript::serialize(google::protobuf::Message& message) const {
        auto& msg = dynamic_cast<serial::Script&>(message);

        auto s_script = msg.mutable_straight_weapon_script();
        s_script->set_deviance(d.stddev());
    }

    std::unique_ptr<Component> StraightWeaponScript::deserialize(const google::protobuf::Message& message) {
        auto& msg = dynamic_cast<const serial::Script&>(message);
        auto p_script = std::make_unique<StraightWeaponScript>(
                *Serializer::currentDeserializationGameObject,
                msg.straight_weapon_script().deviance()
                );
        return p_script;
    }
}
