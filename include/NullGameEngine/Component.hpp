#pragma once

#include <NullGameEngine.hpp>

namespace null {
    class Component {
    private:
        null::GameObject &gameObject;
    public:
        explicit Component(GameObject &gameObject) : gameObject(gameObject) {}
        virtual void start();
        virtual void update();
    };
}

