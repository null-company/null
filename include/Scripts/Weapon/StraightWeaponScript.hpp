#pragma once

#include <Weapon/WeaponScript.hpp>

namespace null {
    class StraightWeaponScript : public WeaponScript {
    private:
        double speed = 1;

    public:
        explicit StraightWeaponScript(GameObject& object);

        void start() override;

        void update() override;

        void shoot(sf::Vector2f from, sf::Vector2f to) override;

        void setWeaponRotation();
    };
}



