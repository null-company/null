#pragma once

#include <GameObject.hpp>
#include <Camera.hpp>

namespace null {
    class CameraScript : public Script {
    public:

        Camera &camera;

        serial::Script prefabSerialize() override;

        explicit CameraScript(Camera &camera);

        static std::unique_ptr<CameraScript> prefabDeserialize(const serial::CameraScript &script);
    };
}