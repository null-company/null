#pragma once


#include <NullGameEngine.hpp>
#include <MainLoop.hpp>

namespace null {

    class Renderer {
        public:
            static void render(const sf::RenderWindow&, const Scene&);
    };

}

