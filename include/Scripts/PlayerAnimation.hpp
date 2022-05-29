#pragma once

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

        void start() override;

        void update() override;

        void damage(float damage);

        PlayerAnimation(GameObject&, SpriteSheet&, const CollisionMap&);

        void serialize(google::protobuf::Message&) const override;

        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);

        GameObject& getGameObject();

        static std::shared_ptr<GameObject> initPlayer(const std::string& anim, b2World& box2dWorld);
    };

}

