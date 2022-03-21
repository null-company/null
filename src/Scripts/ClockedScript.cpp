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

    ClockedScript::ClockedScript(GameObject &gameObject) : Script(gameObject) {}

    serial::Script ClockedScript::prefabSerialize() {
        serial::Script script;
        script.mutable_clocked_script();
        return script;
    }

    std::unique_ptr<ClockedScript> ClockedScript::prefabDeserialize(const serial::ClockedScript &script) {
        return {};
    }

}

