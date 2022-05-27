#pragma once

#include <Script.hpp>
#include <SFML/System.hpp>
#include <GameObject.hpp>

namespace null {

    class ReloadSceneScript : public Script {

    public:
        void start() override;

        void update() override;

        explicit ReloadSceneScript(GameObject &gameObject);

        void serialize(google::protobuf::Message &) const override;
        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);
    };

}

