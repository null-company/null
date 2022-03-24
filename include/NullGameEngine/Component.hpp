#pragma once

#include <NullGameEngine.hpp>
#include "scene.pb.h"
#include "serializable.h"

namespace null {
    class Component : public Serializable {
    protected:
        null::GameObject& gameObject;
    public:
        explicit Component(GameObject &);

        virtual void start() {};

        virtual void update() {};

        void serialize(google::protobuf::Message *message) override { };
        void deserialize(google::protobuf::Message *message) override { };

    };
}

