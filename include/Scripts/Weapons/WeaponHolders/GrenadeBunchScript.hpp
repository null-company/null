#pragma once

#include "WeaponScript.hpp"
#include "Schedulers/LinearScheduler.hpp"

namespace null {
    class GrenadeBunchScript : public WeaponScript {
    private:
        LinearScheduler scheduler{5.f, 20.f, std::chrono::milliseconds{700}};
    public:
        GrenadeBunchScript(GameObject& object);

        void start() override;

        void update() override;

        void shoot(sf::Vector2f from, sf::Vector2f to) override;
    };
}
