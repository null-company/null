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

        serial::Script prefabSerialize() override;

        static std::unique_ptr<ExampleScript> prefabDeserialize(const serial::ExampleScript &script);
    };

}

