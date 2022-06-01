#include <Weapons/WeaponHolders/WeaponScript.hpp>
#include <PlayerAnimation.hpp>

null::WeaponScript::WeaponScript(null::GameObject& object) : Component(object) {}


namespace null {
    bool WeaponScript::checkIfCanShoot() {
        if (isControlledByCurrentPlayer() && clock::now() - lastShot > restartTime) {
            return true;
        }
        return false;
    }

    bool WeaponScript::isControlledByCurrentPlayer() {
        auto parent = gameObject.getParent().lock();
        if (parent == nullptr) {
            return false;
        }
        if (parent->getScript<PlayerAnimation>()
            && parent->getScript<PlayerAnimation>()->controller == PlayerAnimation::Keyboard) {
            return true;
        }
        return false;
    }

    void WeaponScript::saveShotInfo() {
        lastShot = clock::now();
    }

}
