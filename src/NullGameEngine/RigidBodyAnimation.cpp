#include "RigidBodyAnimation.hpp"

namespace null {

    void RigidBodyAnimation::enableFixture(b2Fixture* fixture) {
        b2Filter filter = *((b2Filter *)fixture->GetUserData().pointer);
        //filter.categoryBits = 1;
        //filter.groupIndex = 0;
        //filter.maskBits = 0xffff;
        fixture->SetFilterData(filter);
    }

    void RigidBodyAnimation::disableFixture(b2Fixture* fixture) {
        b2Filter filter;
        filter.categoryBits = 1;
        filter.groupIndex = 0;
        filter.maskBits = 0;
        fixture->SetFilterData(filter);
    }

    void RigidBodyAnimation::update() {
        auto currAnim = spriteSheet.currAnimation->name;
        auto currFrame = spriteSheet.currFrame;
        bool isEnabled = true;
        for (auto* fixture : fixtureMap[currAnim][currFrame]){
            if (fixture->GetFilterData().maskBits == 0) {
                isEnabled = false;
                break;
            }
        }
        if (!isEnabled){
            for (auto& pair: fixtureMap) {
                for (auto& fvec: pair.second) {
                    for (auto* f: fvec) {
                        disableFixture(f);
                    }
                }
            }
            for (auto& f : fixtureMap[currAnim][currFrame]) {
                enableFixture(f);
            }
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
        // disable all current fixtures of the body
        for (auto fx = gameObject.getRigidBody()->GetFixtureList(); fx != nullptr; fx = fx->GetNext()) {
            disableFixture(fx);
        }
        // place b2Filter from fixtures in fixtureDefMap into userData of themselves
        // add them to the body and fixtureMap and disable them
        for (const auto& pair : fixtureDefMap) {
            fixtureMap.insert({pair.first, { }});
            for (auto& fdefvec : pair.second) {
                fixtureMap[pair.first].push_back({});
                for (auto i = 0; i < fdefvec.size(); i++){
                    auto* fdef = new b2FixtureDef(fdefvec[i]);
                    //TODO this has 99% chance to lead to a memory leak, probably needs a fix
                    auto* filter = new b2Filter(fdef->filter);
                    fdef->userData.pointer = (uintptr_t) filter;
                    auto fixture = gameObject.getRigidBody()->CreateFixture(fdef);;
                    fixtureMap[pair.first][fixtureMap[pair.first].size() - 1].push_back(fixture);
                    disableFixture(fixture);
                    delete fdef;
                }
            }
        }
    }
}
