//
// Created by artemonchelus on 12.04.2022.
//

#pragma once


#include "box2d/b2_world_callbacks.h"
#include <vector>
#include <functional>
#include <GameObject.hpp>

namespace null {
    class ContactListener : public b2ContactListener {
    private:
    public:
        void BeginContact(b2Contact* contact) override;

        void EndContact(b2Contact* contact) override;

        void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

        void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
    };
}


