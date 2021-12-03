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

        virtual serial::serialize();
    };
}

