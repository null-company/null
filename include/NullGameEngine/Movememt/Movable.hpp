#pragma once

#include <SFML/Graphics.hpp>

class Movable {
    virtual sf::Vector2f step() = 0;
};
