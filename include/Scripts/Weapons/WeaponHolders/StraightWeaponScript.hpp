#pragma once

#include <SFML/Audio/Sound.hpp>

#include <box2d/box2d.h>

#include <Weapons/WeaponHolders/WeaponScript.hpp>
#include <Network/NetworkManagerClientScript.hpp>
#include <Network/Utility/EveryAcceptedMessageProcessor.hpp>


namespace null {
    class StraightWeaponScript : public WeaponScript {
    private:
        double speed = 5;
        sf::Vector2f initialScale = {1,1};
    public:
        explicit StraightWeaponScript(GameObject& object, double deviance);

        void start() override;

        void update() override;

        void shoot(sf::Vector2f from, sf::Vector2f to) override;

        void setWeaponRotationByMouseCoords(sf::Vector2<float> mouseCoords);

        void serialize(google::protobuf::Message&) const override;
        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);

    private:
        void sendState(sf::Vector2f mousePos, sf::Vector2f weaponEnd, bool isShooting);
        void getStateFromNetAndApply();
        void processClientCommands();
    private:
        enum {
            Server, Client
        } host;
        EveryAcceptedMessageProcessor<net::GameMessage::ClientCommand> serverQueue{};
        EveryAcceptedMessageProcessor<net::GameMessage::SubscriberState> clientQueue{};
        NetworkManagerClientScript* networkManagerScript{};
        sf::Sound* gunShotSound{};
    };
}



