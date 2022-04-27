//
// Created by artemonchelus on 28.04.22.
//

#include "Weapon/WeaponStorage.hpp"

#include <utility>

namespace null {
    int WeaponStorage::counter = 0;

    void null::WeaponStorage::start() {
        Component::start();
        auto weapon = weapons.front();
        gameObject.addChild(std::move(weapon));
    }

    void null::WeaponStorage::update() {
        Component::update();
    }

    null::WeaponStorage::WeaponStorage(null::GameObject& object,
                                       std::vector<std::shared_ptr<GameObject>>& weapons)
            : Component(object) {
        for (auto& weapon: weapons) {
            weapon->addTag("WeaponInStorage:" + std::to_string(WeaponStorage::counter++));
        }
    }

}
