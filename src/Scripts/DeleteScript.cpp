#include "DeleteScript.hpp"
#include "GameObject.hpp"

namespace null {
    void DeleteScript::start() {
        Component::start();
    }

    void DeleteScript::update() {
        Component::update();
        if (isDeleted) {
            gameObject.destroy();
        }
    }

    void DeleteScript::deleteGameObject() {
        isDeleted = true;
    }

    DeleteScript::DeleteScript(GameObject& object) : Component(object) {}
}