#pragma once

#include <vector>
#include <functional>

#include <box2d/b2_world_callbacks.h>
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


