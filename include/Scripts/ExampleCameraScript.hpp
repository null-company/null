#pragma once

#include <Scripts.hpp>

#define q(a) #a

namespace null {
    class ExampleCameraScript : public CameraScript {
    private:
        GameObject* map = nullptr;
        GameObject* trackedObject = &gameObject;
        sf::FloatRect computeNewViewRect();
    public:
        void start() override;

        void update() override;

        explicit ExampleCameraScript(Camera& camera);

        void setTrackedGameObject(GameObject& gameObject);

        void setMap(GameObject& object);


    };
}
