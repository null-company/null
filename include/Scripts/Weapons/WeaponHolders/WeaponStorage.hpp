#pragma once

#include <queue>
#include <memory>

#include <GameObject.hpp>

namespace null {
    class WeaponStorage : public Script {
    private:
        std::queue<std::shared_ptr<GameObject>> weapons;
        std::map<std::shared_ptr<GameObject>, std::string> weaponToTag;
        static int counter;
    public:
        WeaponStorage(GameObject& object, const std::vector<std::shared_ptr<GameObject>>& weapons);

        void start() override;

        void update() override;

        void swapWeapon();

        void addWeapon(const std::shared_ptr<GameObject> &weapon);

        void serialize(google::protobuf::Message & message) const override;

        static std::unique_ptr<Script> deserialize(const google::protobuf::Message& message);
    };
}
