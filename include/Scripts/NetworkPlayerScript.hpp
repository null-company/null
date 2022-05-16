#pragma once

#include <queue>
#include <RigidBodyAnimation.hpp>
#include "serialized/serverConfig.pb.h"
#include <network.h>

namespace null{
    class NetworkPlayerScript : public RigidBodyAnimation {
    private:
        int id;
        std::queue<net::GameMessage>& queue;
        void handleMessage(net::GameMessage& msg);
    public:
        void update() override;
        NetworkPlayerScript(GameObject& gameObject, SpriteSheet& spriteSheet,
                            const CollisionMap& map,
                            std::queue<net::GameMessage>& q, int id);
        // no serialization since it's deprecated
        // TODO delete this script altogether
        void serialize(google::protobuf::Message &) const override {};
        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&) {
            return {};
        };
    };
}

