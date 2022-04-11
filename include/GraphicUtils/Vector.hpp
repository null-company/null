#pragma once

#include <SFML/Graphics.hpp>

namespace null {
    sf::Vector2f normalize(sf::Vector2f vector);

    float getAngle(sf::Vector2f vector);

    sf::Vector2f fromAngle(float angle);
}


