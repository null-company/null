#include "Weapons/WeaponHolders/StraightWeaponScript.hpp"
#include "Script.hpp"
#include "GameObject.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"
#include "box2d/box2d.h"
#include "Serializer.hpp"
#include "Weapons/WeaponAmmunition/BulletScript.hpp"
#include "Graphic/Vector.hpp"
#include "PlayerAnimation.hpp"
#include <SceneLoader.hpp>

namespace null {
    void StraightWeaponScript::start() {
        WeaponScript::start();
        initialScale = gameObject.getSprite().getScale();
        gunShotSound = &ResourceManager::getSound("gunshot.ogg");
        gameObject.getSprite().setOrigin(sf::Vector2f(40, 60));
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
        bool controlled = isControlledByCurrentPlayer();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && checkIfCanShoot()) {
            auto windowInfo = scene->getWindowMetaInfo().absoluteMouseWorldCoords;
            sf::Vector2f weaponDirection = windowInfo - gameObject.getPosition();
            weaponDirection = normalize(weaponDirection);
            weaponDirection *= 120.0f;
            shoot(gameObject.getPosition() + weaponDirection, scene->getWindowMetaInfo().absoluteMouseWorldCoords);
        }
        auto coords = parent->getPosition() + sf::Vector2f(60, 60);
        gameObject.setPosition(coords);
        if (controlled) {
            setWeaponRotation();
        }
    }

    void StraightWeaponScript::setWeaponRotation() {
        auto scene = gameObject.getScene().lock();
        if (scene == nullptr) {
            return;
        }
        auto window_info = scene->getWindowMetaInfo().absoluteMouseWorldCoords;
        sf::Vector2f weapondirection = window_info - gameObject.getPosition();
        gameObject.getSprite().setRotation(getAngle(weapondirection));
        if (getAngle(weapondirection) > 90 and getAngle(weapondirection) < 270) {
            gameObject.getSprite().setScale(initialScale.x, -1.0f * initialScale.y);
        } else {
            gameObject.getSprite().setScale(initialScale.x, initialScale.y);
        }
    }

    void StraightWeaponScript::shoot(sf::Vector2f from, sf::Vector2f to) {
        saveShotInfo();
        static int i = 0;
        auto bullet = std::make_shared<GameObject>();
        bullet->setName(std::string("Bullet: ") + std::to_string(i++));
        bullet->addScript<BulletScript>(*bullet, from, gameObject.getSprite().getRotation() + d(gen), speed);

        gameObject.addChild(std::move(bullet));
        gunShotSound->play();
    }

    StraightWeaponScript::StraightWeaponScript(GameObject& object, double deviance) : WeaponScript(object) {
        d = std::normal_distribution<>(0, deviance);
        sf::Texture* weaponTexture = ResourceManager::loadTexture("weapon.png");
//        gameObject.getSprite().setOrigin(sf::Vector2f(250, 100));
        gameObject.getSprite().setTexture(*weaponTexture);
        gameObject.getSprite().scale(0.40, 0.25);
        speed = 8;
        gameObject.renderLayer = serial::FOREGROUND2;
        gameObject.visible = true;
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
