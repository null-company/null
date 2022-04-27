#pragma once

#include <Movememt/Movable.hpp>

class LinearMovable : public Movable {
private:
    sf::Vector2f from, to;
    /**
     * Speed is measured in pixels per millisecond
     */
    double speed;
public:
    sf::Vector2f step() override;
};
