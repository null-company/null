#pragma once

#include <memory>
#include <vector>

#include <Script.hpp>
#include <GameObject.hpp>
#include <Schedulers/Timer.hpp>

namespace null {

    class WeaponGenerator : public Script {
        Timer timer{std::chrono::milliseconds{3000}};
    public:

        explicit WeaponGenerator(GameObject& object);

        void start() override;

        void update() override;

        std::vector<std::shared_ptr<GameObject>> getAllPlatforms();

        std::shared_ptr<GameObject> generateWeapon();

        void addWeaponToPlatform();

        void serialize(google::protobuf::Message & message) const override;

        static std::unique_ptr<Script> deserialize(const google::protobuf::Message& message);
    };

}
