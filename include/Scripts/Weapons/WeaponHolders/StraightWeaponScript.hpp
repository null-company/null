#pragma once

#include "WeaponScript.hpp"
#include "box2d/box2d.h"

namespace null {
    class StraightWeaponScript : public WeaponScript {
    private:
        double speed = 5;

    public:
        explicit StraightWeaponScript(GameObject& object, double deviance);

        void start() override;

        void update() override;

        void shoot(sf::Vector2f from, sf::Vector2f to) override;

        void setWeaponRotation();

        void serialize(google::protobuf::Message &) const override;
        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);
    };
}



