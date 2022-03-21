#include "CameraScript.hpp"
#include <Camera.hpp>

namespace null {
    CameraScript::CameraScript(Camera &camera) : Component(camera), camera(camera) {}

}
