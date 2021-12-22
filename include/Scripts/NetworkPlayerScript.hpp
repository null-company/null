#pragma once

#include <queue>
#include <RigidBodyAnimation.hpp>

namespace null{
    class NetworkPlayerScript : public RigidBodyAnimation {
    private:
        int id;
        std::queue<net::GameMessage>& queue;
        void handleMessage(net::GameMessage& msg);
    public:
        void update() override;
        NetworkPlayerScript(GameObject& gameObject, SpriteSheet& spriteSheet,
                            std::unordered_map<std::string, std::vector<std::vector<b2FixtureDef>>> map,
                            std::queue<net::GameMessage>& q, int id);

        serial::Script prefabSerialize() override;
    };
}

