#pragma once

#include <SFML/System.hpp>

#include <GameObject.hpp>

namespace null {

    class ExampleScript : public Script {
        sf::Clock clock;
        
        public:
        void start() override;

        void update() override;

        explicit ExampleScript(GameObject& gameObject);

    };

}

