#pragma once

#include <SFML/Audio/Sound.hpp>

#include <RigidBodyAnimation.hpp>

namespace null {
    /**
     * :field controlled - set true if player must be controlled by keyboard, false otherwise
     */
    class PlayerAnimation : public RigidBodyAnimation {
    public:
        bool moovin = false;
        int fram = 0;
        bool canJump = false;
        sf::Vector2f previousPosition = {0, 0};
        bool flip = false;
        bool controlled = false;
        std::string name = "default";
        float health = 100;

    public:
        void start() override;

        void update() override;

        void damage(float damage);

        PlayerAnimation(GameObject&, SpriteSheet&, const CollisionMap&);

        void serialize(google::protobuf::Message&) const override;

        GameObject& getGameObject();

        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);

        static std::shared_ptr<GameObject> initPlayer(const std::string& anim, b2World& box2dWorld);

    private:
        sf::Sound* deathSound{};
        sf::Sound* jumpSound{};
    };
}
