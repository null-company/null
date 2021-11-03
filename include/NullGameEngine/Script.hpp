#pragma once

#include <NullGameEngine.hpp>
#include <Component.hpp>

namespace null {
    class Script : public Component{
    public:
        virtual void start();
        virtual void update();
    };
}
