#include "Weapons/WeaponHolders/WeaponStorage.hpp"
#include "Serializer.hpp"

#include <utility>

#include <Scene.hpp>
#include <GameObject.hpp>

namespace null {
    int WeaponStorage::counter = 0;

    void WeaponStorage::start() {
        Component::start();
        if (!weapons.empty()) {
            auto weapon = weapons.front();
            gameObject.addChild(weapon);
        }
        gameObject.setPosition(gameObject.getParent().lock()->getPosition());
    }

    void WeaponStorage::update() {
        Component::update();
        gameObject.setPosition(gameObject.getParent().lock()->getPosition());
        if (gameObject.getSceneForce().windowMetaInfo.getKeyEvent().code == sf::Keyboard::E) {
            swapWeapon();
        }
    }

    WeaponStorage::WeaponStorage(null::GameObject& object,
                                 const std::vector<std::shared_ptr<GameObject>>& weapons)
            : Component(object) {
        gameObject.addTag("WeaponStorage");
        for (auto& weapon: weapons) {
            addWeapon(weapon);
        }
    }

    void WeaponStorage::addWeapon(const std::shared_ptr<GameObject>& weapon) {
        auto tag = "WeaponInStorage:" + std::to_string(WeaponStorage::counter++);
        weapon->addTag(tag);
        this->weapons.push(weapon);
        weaponToTag.insert(std::pair{weapon, tag});
    }

    void WeaponStorage::swapWeapon() {
        if(weapons.empty()) {
            return;
        }
        auto weapon = gameObject.findFirstChildrenByTag(weaponToTag[weapons.front()]);
        if (weapon != nullptr) {
            gameObject.deleteChild(&*weapon);
            weapons.push(weapon);
            weapons.pop();
        }
        auto newWeapon = weapons.front();
        gameObject.addChild(newWeapon);
    }

    void WeaponStorage::serialize(google::protobuf::Message& message) const {
        auto& msg = (serial::Script&)message;
        msg.mutable_weapon_storage();
    }

    std::unique_ptr<Script> WeaponStorage::deserialize(const google::protobuf::Message& message) {
        auto vec = std::vector<std::shared_ptr<GameObject>>{};
        auto p_script = std::make_unique<WeaponStorage>(
                *Serializer::currentDeserializationGameObject,
                vec
        );
        return p_script;
    }


}
