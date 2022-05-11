//
// Created by artemonchelus on 11.05.22.
//

#include "DeleteScript.hpp"
#include "GameObject.hpp"

namespace null {
    void DeleteScript::start() {
        Component::start();
    }

    void DeleteScript::update() {
        Component::update();
        if (isDeleted) {
            gameObject.deleteMe();
        }
    }

    void DeleteScript::del() {
        isDeleted = true;
    }

    DeleteScript::DeleteScript(GameObject& object) : Component(object) {}
}