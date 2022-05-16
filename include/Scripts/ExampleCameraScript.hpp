#pragma once

#include <Scripts.hpp>

#define q(a) #a

namespace null {
    class ExampleCameraScript : public CameraScript {
    private:
        sf::FloatRect computeNewViewRect();
    public:
        GameObject* map = nullptr;
        GameObject* trackedObject = &gameObject;
        void start() override;

        void update() override;

        explicit ExampleCameraScript(GameObject& camera);

        void setTrackedGameObject(GameObject& gameObject);

        void setMap(GameObject& object);

        void serialize(google::protobuf::Message &) const override;
        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);

    };
}
