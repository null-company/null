#pragma once

#include <SFML/System.hpp>

#include <GameObject.hpp>

namespace null {

    class ExampleScript : public Script {
        sf::Clock clock{};
        
        public:
        void start() override;

        void update() override;

        explicit ExampleScript(GameObject& gameObject);

        void serialize(google::protobuf::Message &) const override;
        static std::unique_ptr<Component> deserialize(const google::protobuf::Message&);
    };

}

