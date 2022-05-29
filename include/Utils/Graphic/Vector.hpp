#pragma once

#include <SFML/Graphics.hpp>

namespace null {
    sf::Vector2f normalize(sf::Vector2f vector);

    float getAngle(sf::Vector2f vector);

    sf::Vector2f fromAngle(float angle);

    sf::Vector2f getScaleToFitNewSize(sf::Vector2f currSize, sf::Vector2f newSize);

    template<typename T>
    sf::Vector2f getScaleToFitNewSize(sf::Vector2<T> currSize, sf::Vector2<T> newSize) {
        return sf::Vector2f{float(newSize.x / currSize.x), float(newSize.y / currSize.y)};
    }
}
