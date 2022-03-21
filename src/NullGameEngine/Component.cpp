#include <Component.hpp>
#include "Scripts.hpp"

namespace null {

    Component::Component(GameObject &gameObject) : gameObject(gameObject) {}

    std::unique_ptr<Component> Component::prefabDeserialize(const serial::Script &script) {
        switch (script.script_instance_case()) {
            case serial::Script::kBasicScript:
                throw std::invalid_argument("Script is just virtual class");
                break;
            case serial::Script::kCameraScript:
                return CameraScript::prefabDeserialize(script.camera_script());
                break;
            case serial::Script::kClockedScript:
//                return ClockedScript::prefabDeserialize(script.clocked_script());
                break;
            case serial::Script::kReloadScript:
                return ReloadSceneScript::prefabDeserialize(script.reload_script());
                break;
            case serial::Script::kExampleCameraScript:
                return ExampleCameraScript::prefabDeserialize(script.example_camera_script());
                break;
            case serial::Script::kExampleClockedScript:
//                return ExampleClockedScript::prefabDeserialize(script.example_clocked_script());
                break;
            case serial::Script::kExampleScript:
//                return ExampleScript::prefabDeserialize(script.example_script());
                break;
            case serial::Script::SCRIPT_INSTANCE_NOT_SET:
                break;
        }
        throw std::invalid_argument("Unknown script");
        return {};
    }

}

