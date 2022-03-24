#pragma once

#include <NullGameEngine.hpp>
#include "scene.pb.h"
#include "serializable.h"

namespace null {
    class Component {
    protected:
        null::GameObject& gameObject;
    public:
        explicit Component(GameObject &);

        virtual void start() { };

        virtual void update() { };

        void serialize(google::protobuf::Message *) { };
        static std::unique_ptr<Component> deserialize(google::protobuf::Message *) {
            return nullptr;
        };

    };
}

