#include <Weapon/WeaponScript.hpp>

null::WeaponScript::WeaponScript(null::GameObject& object) : Component(object) {}


namespace null {
    bool WeaponScript::checkIfCanShoot() {
        if (clock::now() - lastShot > restartTime) {
            return true;
        }
        return false;
    }

    void WeaponScript::saveShotInfo() {
        lastShot = clock::now();
    }

}
