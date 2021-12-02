#pragma once

#include <NullGameEngine.hpp>
#include <GameObject.hpp>
#include <SFML/System.hpp>
#include <CameraScript.hpp>

namespace null {

    class Camera final : public GameObject {
    public:
        sf::View view;
    };
}

