#pragma once

#include <NullGameEngine.hpp>
#include "scene.pb.h"

namespace null {
    class Component {
    protected:
        null::GameObject &gameObject;
    public:
        Component(GameObject &);

        virtual void start() {};

        virtual void update() {};

        //TODO Should it be pure virtual?
        virtual serial::Script prefabSerialize() = 0;

        static std::unique_ptr<Component> prefabDeserialize(const serial::Script &script);
    };
}

