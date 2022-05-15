#pragma once

#include <RigidBodyAnimation.hpp>

namespace null {
    class PlayerAnimation : public RigidBodyAnimation {
    public:
        bool flip = false;

        void start() override;
        void update() override;
        PlayerAnimation(GameObject&, SpriteSheet&, const CollisionMap&);

        void serialize(google::protobuf::Message &) const override;
        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);
    };

}

