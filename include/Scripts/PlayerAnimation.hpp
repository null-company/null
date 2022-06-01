#pragma once

#include <SFML/Audio/Sound.hpp>

#include <RigidBodyAnimation.hpp>
#include <Network/PrimitiveStateConverter.hpp>
#include <Network/Utility/LastAcceptedMessageProcessor.hpp>
#include <Network/NetworkManagerClientScript.hpp>

namespace null {

    class PlayerAnimation : public RigidBodyAnimation {
    public:
        enum Controller: uint8_t {
//            Nothing = 0,
            Keyboard = 1,
            Network = 2
        };
    public:
        std::string name = "default";
        Controller controller = Network;

    public:
        void start() override;

        void update() override;

        void damage(float damage);

        [[nodiscard]]
        float getHealth() const;

        PlayerAnimation(GameObject&, SpriteSheet&, const CollisionMap&);

        void serialize(google::protobuf::Message&) const override;

        GameObject& getGameObject();

        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);

        static std::shared_ptr<GameObject> initPlayer(const std::string& anim, b2World& box2dWorld);

    private:
        float health = 100;
        int currentAnimationFrame = 0;
        bool canJump = false;
        sf::Vector2f previousPosition = {0, 0};

        enum {
            Server, Client
        } host;

        LastAcceptedMessageProcessor<net::GameMessage::ClientCommand> serverQueue{};
        LastAcceptedMessageProcessor<net::GameMessage::SubscriberState> clientQueue{};
        NetworkManagerClientScript* networkManagerScript{};

        sf::Sound* deathSound{};
        sf::Sound* jumpSound{};
    };
}
