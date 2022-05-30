#pragma once

#include <SFML/Audio/Sound.hpp>

#include <box2d/box2d.h>

#include <Weapons/WeaponHolders/WeaponScript.hpp>


namespace null {
    class StraightWeaponScript : public WeaponScript {
    public:
        explicit StraightWeaponScript(GameObject& object, double deviance);

        void start() override;

        void update() override;

        void shoot(sf::Vector2f from, sf::Vector2f to) override;

        void setWeaponRotation();

        void serialize(google::protobuf::Message&) const override;
        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);

    private:
        double speed = 5;
        sf::Sound* gunShotSound{};
    };
}



