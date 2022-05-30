#pragma once

#include <Scripts.hpp>

namespace null {
    class ExampleCameraScript : public CameraScript {
    private:
        sf::FloatRect computeNewViewRect();

    public:
        float scale = 1;
        GameObject* map = nullptr;
        GameObject* trackedObject = &gameObject;

        void start() override;

        void update() override;

        explicit ExampleCameraScript(GameObject& camera);

        void setTrackedGameObject(GameObject& gameObject);

        void setMap(GameObject& object);

        void serialize(google::protobuf::Message&) const override;

        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);

        float getScale() const;

        void setScale(float scale);
    };
}
