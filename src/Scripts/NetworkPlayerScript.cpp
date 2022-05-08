#include <NetworkPlayerScript.hpp>
#include "serialized/serverConfig.pb.h"

namespace null {


    NetworkPlayerScript::NetworkPlayerScript(GameObject& gameObject, SpriteSheet& spriteSheet,
                                             std::unordered_map<std::string, std::vector<std::vector<b2FixtureDef>>> map,
                                             std::queue<net::GameMessage>& q, int id) :
            RigidBodyAnimation(gameObject, spriteSheet, map), queue(q), id(id) {}

    void NetworkPlayerScript::update() {
        net::GameMessage message;
        bool bruhmomento = false;
        for (auto m = queue.front(); !queue.empty(); m = queue.front(), queue.pop()) {
            //if (m.game_id() == id) {
            message = m;
            bruhmomento = true;
            //}
        }
        if (bruhmomento) {
            handleMessage(message);
        }
        RigidBodyAnimation::update();
    }

    void NetworkPlayerScript::handleMessage(net::GameMessage& msg) {
        float x = msg.player_info().x();
        float y = msg.player_info().y();
        std::string currAnim = msg.player_info().curranim();
        int currFrame = msg.player_info().currframe();
        gameObject.getRigidBody()->SetTransform({x, y}, gameObject.getRigidBody()->GetAngle());
        spriteSheet.setAnimation(currAnim);
        spriteSheet.setFrame(currFrame);
    }
}