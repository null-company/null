#pragma once

#include <SFML/System.hpp>

#include <Script.hpp>

namespace null {

    /*
     * ClockedScript is meant to be subclassed by scripts
     * that use clock on every update (for physics, presumably)
     * Instead of overriding start and update, override
     * clockedStart and clockedUpdate
     * In clockedUpdate, use ClockedScript::elapsed. It is the amount of
     * time since the last update
     */
    class ClockedScript : public Script {
        sf::Clock clock;

    protected:
        sf::Time elapsed;

        virtual void clockedStart() = 0;

        virtual void clockedUpdate() = 0;

    public:
        void start() override final;

        void update() override final;

        explicit ClockedScript(GameObject &gameObject);

    };

}

