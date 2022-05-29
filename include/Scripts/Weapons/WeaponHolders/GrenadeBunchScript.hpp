#pragma once

#include <Weapons/WeaponHolders/WeaponScript.hpp>
#include <Schedulers/LinearScheduler.hpp>

namespace null {
    class GrenadeBunchScript : public WeaponScript {
    private:
        LinearScheduler scheduler{5.f, 20.f, std::chrono::milliseconds{700}};
    public:
        explicit GrenadeBunchScript(GameObject& object);

        void start() override;

        void update() override;

        void shoot(sf::Vector2f from, sf::Vector2f to) override;

        void serialize(google::protobuf::Message & message) const override;

        static std::unique_ptr<Script> deserialize(const google::protobuf::Message& message);
    };
}
