#pragma once

#include <Weapons/WeaponHolders/WeaponScript.hpp>
#include <Schedulers/LinearScheduler.hpp>
#include <Network/Utility/EveryAcceptedMessageProcessor.hpp>
#include <Network/NetworkManagerClientScript.hpp>

namespace null {
    class GrenadeBunchScript : public WeaponScript {
    public:
        explicit GrenadeBunchScript(GameObject& object);

        void start() override;

        void update() override;

        void shoot(sf::Vector2f from, sf::Vector2f to) override;
    private:
        enum {
            Server, Client
        } host;


        void processClientCommands();
        void sendState(sf::Vector2<float> vector2, bool b);
        void getStateFromNetAndApply();

        EveryAcceptedMessageProcessor<net::GameMessage::ClientCommand> serverQueue{};
        EveryAcceptedMessageProcessor<net::GameMessage::SubscriberState> clientQueue{};
        NetworkManagerClientScript* networkManagerScript{};
        LinearScheduler scheduler{5.f, 20.f, std::chrono::milliseconds{700}};
    };
}
