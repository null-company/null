#pragma once

#include <Script.hpp>
#include <SFML/System.hpp>
#include <GameObject.hpp>

namespace null {

    class ReloadSceneScript : public Script {

    public:
        void start() override;

        void update() override;

        explicit ReloadSceneScript(GameObject &gameObject);

    };

}

