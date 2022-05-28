#pragma once

#include <GameObject.hpp>
#include <Camera.hpp>
#include <WindowMetaInfo.hpp>

namespace null {
    class CameraScript : public Script {
    public:
        WindowMetaInfo* windowMetaInfo;
        GameObject& camera;
        sf::View view;
        explicit CameraScript(GameObject& camera);

        void start() override;

        void update() override;

        void serialize(google::protobuf::Message &) const override;
        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);
    };
}