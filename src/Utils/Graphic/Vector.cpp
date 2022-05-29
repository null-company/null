#include "Graphic/Vector.hpp"
#include <cmath>

namespace null {
    sf::Vector2f normalize(sf::Vector2f vector) {
        float vLen = std::sqrt(vector.x * vector.x + vector.y * vector.y);
        vector /= vLen;
        return vector;
    }

    float getAngle(sf::Vector2f vector) {
        float delta = 0;
        if (vector.x < 0) {
            delta = 180.0f;
        }

        return delta + std::atan(vector.y / vector.x) * 180.0f / 3.141592f;
    }

/**
 * @return vector with len = 1 and provided angle
 */
    sf::Vector2f fromAngle(float angle) {
        sf::Vector2f v;
        v.y = std::tan(angle);
        v.x = 1;
        return normalize(v);
    }


}

