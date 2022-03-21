#include "CameraScript.hpp"
#include <Camera.hpp>

namespace null {
    CameraScript::CameraScript(Camera &camera) : Component(camera), camera(camera) {}

    serial::Script CameraScript::prefabSerialize() {
        serial::Script script;
        script.mutable_camera_script();
        return script;
    }

    std::unique_ptr<CameraScript> CameraScript::prefabDeserialize(const serial::CameraScript &script) {
        return {};
    }

}
