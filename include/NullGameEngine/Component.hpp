#pragma once

#include <NullGameEngine.hpp>

namespace null {
    class Component {
    protected:
        null::GameObject &gameObject;
    public:
        Component(GameObject &gameObject) : gameObject(gameObject) { }
        virtual void start() { };
        virtual void update() { };
    };
}

