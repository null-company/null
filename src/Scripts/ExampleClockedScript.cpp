#include <ExampleClockedScript.hpp>
#include <Script.hpp>
#include <GameObject.hpp>
#include <SFML/System.hpp>
#include "Serializer.hpp"


namespace null {

    void ExampleClockedScript::clockedStart() {}

    void ExampleClockedScript::clockedUpdate() {
        const sf::Vector2f &position = gameObject.getPosition();
        sf::Vector2f newPosition = position;
        newPosition.x += elapsed.asMilliseconds() * 3.0f;
        gameObject.setPosition(newPosition);
    }

    ExampleClockedScript::ExampleClockedScript(GameObject &gameObject)
            : ClockedScript(gameObject) {}

    void ExampleClockedScript::serialize(google::protobuf::Message& message) const {
        auto& msg = dynamic_cast<serial::Script&>(message);
        msg.mutable_example_clocked_script();
    }

    std::unique_ptr<Component> ExampleClockedScript::deserialize(const google::protobuf::Message& message) {
        auto& msg = dynamic_cast<const serial::Script&>(message);
        auto p_script = std::make_unique<ExampleClockedScript>(
                *Serializer::currentDeserializationGameObject
                );
        return p_script;
    }

}

