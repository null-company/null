#include "NetworkPlayerScript.hpp"

namespace null {


    NetworkPlayerScript::NetworkPlayerScript(GameObject&gameObject, SpriteSheet& spriteSheet,
                                             std::unordered_map<std::string, std::vector<std::vector<b2FixtureDef>>> map,
                                             std::queue<int>& q) :
                                             RigidBodyAnimation(gameObject, spriteSheet, map), queue(q) { }

    void NetworkPlayerScript::update() {
        for (auto m = queue.front(); !queue.empty(); queue.pop(), m = queue.front()) {
            handleMessage(m);
        }
        RigidBodyAnimation::update();
    }

    void NetworkPlayerScript::handleMessage(int msg) {
        float x = 0;
        float y = 0;
        std::string currAnim;
        int currFrame = 0;
        gameObject.getRigidBody()->SetTransform({x, y}, gameObject.getRigidBody()->GetAngle());
        spriteSheet.setAnimation(currAnim);
        spriteSheet.setFrame(currFrame);
    }
}