#pragma once

#include <queue>
#include <RigidBodyAnimation.hpp>

namespace null{
    class NetworkPlayerScript : public RigidBodyAnimation {
    private:
        std::queue<int>& queue;
        void handleMessage(int);
    public:
        //void start();
        void update() override;
        NetworkPlayerScript(GameObject&, SpriteSheet&, std::unordered_map<std::string, std::vector<std::vector<b2FixtureDef>>>, std::queue<int>&);
    };
}

