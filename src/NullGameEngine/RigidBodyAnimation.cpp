#include "RigidBodyAnimation.hpp"

namespace null {

    void RigidBodyAnimation::update() {
        auto currAnim = spriteSheet.currAnimation->name;
        auto currFrame = spriteSheet.currFrame;
        if (currAnim != currRigidAnim || currFrame != currRigidFrame) {
            currRigidAnim = currAnim;
            currRigidFrame = currFrame;
            destroyAllFixtures();
            createFixtures();
        }

        Animation::update();
    }

    RigidBodyAnimation::RigidBodyAnimation(GameObject& gameObject, SpriteSheet& spriteSheet,
                                           const std::unordered_map<std::string, std::vector<std::vector<b2FixtureDef>>>& fixtureDefMap) :
            Animation(gameObject, spriteSheet) {
        // check that spriteSheet and fixtureDefMap are the same size
        if (fixtureDefMap.size() != spriteSheet.animations.size()) {
            throw std::invalid_argument("RigidBodyAnimation error: fixtureDefMap and spriteSheet are not the same size");
        }
        for (const auto& nameFixturePair : fixtureDefMap) {
            if (nameFixturePair.second.size() != spriteSheet.animations[nameFixturePair.first].framePositions.size()) {
                throw std::invalid_argument("RigidBodyAnimation error: fixtureDefMap and spriteSheet are not the same size");
            }
        }

        destroyAllFixtures();

        for (const auto& pair : fixtureDefMap) {
            this->fixtureDefMap.insert(pair);
        }
    }

    void RigidBodyAnimation::destroyAllFixtures() {
        for (auto fx = gameObject.getRigidBody()->GetFixtureList(); fx != nullptr; fx = fx->GetNext()) {
            gameObject.getRigidBody()->DestroyFixture(fx);
        }
    }

    void RigidBodyAnimation::createFixtures() {
        for (auto& fd : fixtureDefMap[currRigidAnim][currRigidFrame]) {
            gameObject.getRigidBody()->CreateFixture(&fd);
        }
    }
}
