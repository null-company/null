#pragma once

#include <GameObject.hpp>
#include <Camera.hpp>

namespace null {
    class CameraScript : public Script {
    public:
        Camera& camera;

        explicit CameraScript(Camera& camera);
    };
}