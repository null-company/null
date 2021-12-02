#pragma once

#include <Scripts.hpp>

namespace null {
    class ExampleCameraScript : public CameraScript {
    public:
        void start() override;
        void update() override;
        explicit ExampleCameraScript(Camera& camera);
    };

}
