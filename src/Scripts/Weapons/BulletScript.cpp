
#include "Component.hpp"
#include "Weapon/BulletScript.hpp"
#include "GameObject.hpp"

namespace null{
    void null::BulletScript::start() {
        Component::start();
    }

    void null::BulletScript::update() {
        Component::update();
    }

    BulletScript::BulletScript(GameObject& object) : Component(object) {}
}

