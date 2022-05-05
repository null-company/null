#pragma once

#include <queue>
#include "GameObject.hpp"
#include <memory>

namespace null {
    class WeaponStorage : public Script {
    private:
        std::queue<std::shared_ptr<GameObject>> weapons;
        std::map<std::shared_ptr<GameObject>, std::string> weaponToTag;
        static int counter;
    public:
        WeaponStorage(GameObject& object, std::vector<std::shared_ptr<GameObject>> weapons);
        void start() override;

        void update() override;

        void swapWeapon();
    };
}
