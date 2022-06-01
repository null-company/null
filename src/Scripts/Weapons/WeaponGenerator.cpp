#include "Weapons/WeaponGenerator.hpp"
#include "Serializer.hpp"

#include <random>

#include <Scene.hpp>
#include <Weapons/WeaponHolders/StraightWeaponScript.hpp>
#include <Weapons/WeaponAmmunition/GrenadeScript.hpp>
#include <GameObjectSensor.hpp>
#include <Weapons/WeaponHolders/GrenadeBunchScript.hpp>
#include <DeleteScript.hpp>

namespace null {
    void WeaponGenerator::start() {
        Component::start();
        timer.start();
    }

    void WeaponGenerator::update() {
        if (timer.expired()) {
            addWeaponToPlatform();
            timer.start();
        }
        Component::update();
    }

    std::vector<std::shared_ptr<GameObject>> WeaponGenerator::getAllPlatforms() {
        auto platforms = gameObject.getSceneForce().findAllByTag("platform");
        std::vector<std::shared_ptr<GameObject>> result;
        for (auto& platform: platforms) {
            result.push_back(platform.lock());
        }
        return result;
    }

    enum WeaponType {
        GUN,
        GRENADE
    };

    std::shared_ptr<GameObject> WeaponGenerator::generateWeapon() {
        std::random_device rd;
        std::mt19937 mt(rd());
        int total_weapons = 2;
        std::uniform_int_distribution<> dist(0, total_weapons - 1);
        int weapon_number = dist(mt);
        std::shared_ptr<GameObject> weapon;
        switch (weapon_number) {
            case WeaponType::GUN: {
                weapon = std::make_shared<GameObject>();
                weapon->addScript<StraightWeaponScript>(*weapon, 0.01);
                break;
            }
            case WeaponType::GRENADE: {
                weapon = std::make_shared<GameObject>();
                weapon->addScript<GrenadeBunchScript>(*weapon);
                break;
            }
            default:
                return nullptr;
        }
        weapon->addTag("weapon");
        auto sensor = std::make_shared<GameObject>();
        sensor->addScript<GameObjectSensor>(*sensor, sf::Vector2f{30, -30}, weapon, onPlayerEnter);
        sensor->addScript<DeleteScript>(*sensor);
        return sensor;
    }

    void WeaponGenerator::addWeaponToPlatform() {
        std::random_device rd;
        std::mt19937 mt(rd());
        auto platforms = getAllPlatforms();
        if (platforms.empty()) {
            return;
        }
        std::uniform_int_distribution<> dist(0, platforms.size() - 1);
        int platformNum = dist(mt);
        auto platform = platforms[platformNum];
        auto generatedWeapon = generateWeapon();
        platform->addChild(generatedWeapon);
    }

    WeaponGenerator::WeaponGenerator(GameObject& object) : Component(object) {}

    void WeaponGenerator::serialize(google::protobuf::Message& message) const {
        auto& msg = (serial::Script&)message;
        msg.mutable_weapon_generator();
    }

    std::unique_ptr<Script> WeaponGenerator::deserialize(const google::protobuf::Message& message) {
        auto p_script = std::make_unique<WeaponGenerator>(
                *Serializer::currentDeserializationGameObject
        );
        return p_script;
    }

} // null