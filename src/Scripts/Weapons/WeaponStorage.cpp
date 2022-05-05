//
// Created by artemonchelus on 28.04.22.
//

#include "Weapon/WeaponStorage.hpp"
#include "Scene.hpp"
#include <utility>
#include <GameObject.hpp>

namespace null {
    int WeaponStorage::counter = 0;

    void WeaponStorage::start() {
        Component::start();
        auto weapon = weapons.front();
        gameObject.addChild(std::move(weapon));
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
                                 std::vector<std::shared_ptr<GameObject>> weapons)
            : Component(object) {
        for (auto& weapon: weapons) {
            auto tag = "WeaponInStorage:" + std::to_string(WeaponStorage::counter++);
            weapon->addTag(tag);
            this->weapons.push(weapon);
            weaponToTag.insert(std::pair{weapon, tag});
        }
    }

    void WeaponStorage::swapWeapon() {
        auto weapon = gameObject.findFirstChildrenByTag(weaponToTag[weapons.front()]);
        gameObject.deleteChild(&*weapon);
        weapons.push(weapon);
        weapons.pop();
        auto newWeapon = weapons.front();
        gameObject.addChild(std::move(newWeapon));
    }


}
