#pragma once

#include <queue>
#include <memory>

#include <GameObject.hpp>
#include "Network/NetworkManagerClientScript.hpp"
#include <Network/Utility/EveryAcceptedMessageProcessor.hpp>
#include <PlayerAnimation.hpp>

namespace null {
    class WeaponStorage : public Script {
    public:
        WeaponStorage(GameObject& object, const std::vector<std::shared_ptr<GameObject>>& weapons);

        void start() override;

        void update() override;

        void swapWeapon();

        void addWeapon(const std::shared_ptr<GameObject> &weapon);

        bool isControlledByCurrentPlayer() {
            auto parent = gameObject.getParent().lock();
            if (parent == nullptr) {
                return false;
            }
            if (parent->getScript<PlayerAnimation>()
                && parent->getScript<PlayerAnimation>()->controller == PlayerAnimation::Keyboard) {
                return true;
            }
            return false;
        }

    private:
        void sendSwapCommand();
        void getStateFromNetAndApply();
        void processClientCommands();
    private:
        enum {
            Server, Client
        } host{Client};
        EveryAcceptedMessageProcessor<net::GameMessage::ClientCommand> serverQueue{};
        EveryAcceptedMessageProcessor<net::GameMessage::SubscriberState> clientQueue{};
        NetworkManagerClientScript* networkManagerScript{};

        std::queue<std::shared_ptr<GameObject>> weapons;
        std::map<std::shared_ptr<GameObject>, std::string> weaponToTag;
        static int counter;
    };
}
