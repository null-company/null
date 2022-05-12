#pragma once

#include <NullGameEngine.hpp>
#include "scene.pb.h"
#include "serializable.h"
#include "Entity.hpp"

namespace null {
    class Component : public Entity {
    protected:
        null::GameObject& gameObject;
    public:

        explicit Component(GameObject &);

        virtual void start() { };

        virtual void update() { };

        // These two functions have to be implemented by subclasses

        virtual void serialize(google::protobuf::Message &) const { };
        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&) {
            return nullptr;
        };

    };
}

