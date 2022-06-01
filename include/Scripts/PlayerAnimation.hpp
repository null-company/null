#pragma once

#include <SFML/Audio/Sound.hpp>

#include <unordered_map>

#include <RigidBodyAnimation.hpp>
#include <Network/PrimitiveStateConverter.hpp>
#include <Network/Utility/LastAcceptedMessageProcessor.hpp>
#include <Network/NetworkManagerClientScript.hpp>

namespace null {

    class PlayerAnimation : public RigidBodyAnimation {
    public:
        enum Controller: uint8_t {
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

    protected:
        enum KeyboardCommand {
            LEFT, RIGHT, JUMP
        };
        std::unordered_map<sf::Keyboard::Key, KeyboardCommand> controls{
                {sf::Keyboard::A, LEFT},
                {sf::Keyboard::D, RIGHT},
                {sf::Keyboard::Space, JUMP},
                {sf::Keyboard::W, JUMP}
        };
    private:
        std::vector<PlayerAnimation::KeyboardCommand> getCurrentCommands();
        void processCommand(PlayerAnimation::KeyboardCommand command);
        void die();
        void sendState();
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
        sf::Clock lastStateSnapshotTimer{};

        sf::Sound* deathSound{};
        sf::Sound* jumpSound{};
    };

    class ArrowsControlledPlayer : public PlayerAnimation {
    public:
        ArrowsControlledPlayer(GameObject& go, SpriteSheet& ss, const CollisionMap& cm)
                : PlayerAnimation(go, ss, cm) {
            controls = {
                    {sf::Keyboard::Left, LEFT},
                    {sf::Keyboard::Right, RIGHT},
                    {sf::Keyboard::Up, JUMP}
            };
        };

        static std::shared_ptr<GameObject> initPlayer(const std::string& anim, b2World& box2dWorld);

    };
}
