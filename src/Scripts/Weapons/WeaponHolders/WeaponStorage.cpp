#include "Weapons/WeaponHolders/WeaponStorage.hpp"

#include <utility>

#include <Scene.hpp>
#include <GameObject.hpp>

namespace null {
    int WeaponStorage::counter = 0;

    void WeaponStorage::start() {
        Component::start();
        if (!weapons.empty()) {
            auto weapon = weapons.front();
            gameObject.getParent().lock()->addChild(std::move(weapon));
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
        if (weapons.empty()) {
            return;
        }
        auto weapon = gameObject.getParent().lock()->findFirstChildrenByTag(weaponToTag[weapons.front()]);
        if (weapon != nullptr) {
            gameObject.getParent().lock()->deleteChild(&*weapon);
            weapons.push(weapon);
            weapons.pop();
        }
        auto newWeapon = weapons.front();
        gameObject.getParent().lock()->addChild(std::move(newWeapon));
    }


}
