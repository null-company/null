#pragma once

#include <SFML/System.hpp>

#include <NullGameEngine.hpp>
#include <GameObject.hpp>
#include <CameraScript.hpp>

namespace null {

    class Camera final : public GameObject {
    public:
        sf::View view;
        Camera();
    };
}

