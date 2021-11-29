#include <SFML/System.hpp>

#include <Script.hpp>
#include <GameObject.hpp>
#include <ClockedScript.hpp>

namespace null {

    void ClockedScript::start() {
        clock.restart();
        clockedStart();
    }

    void ClockedScript::update() {
        elapsed = clock.restart();
        clockedUpdate();
    }

    ClockedScript::ClockedScript(GameObject& gameObject) : Script(gameObject) {  }

}

