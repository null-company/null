#include <ExampleClockedScript.hpp>
#include <Script.hpp>
#include <Scripts.hpp>
#include <GameObject.hpp>
#include <SFML/System.hpp>


namespace null {

    void ExampleClockedScript::clockedStart() { }
    void ExampleClockedScript::clockedUpdate() {
        const sf::Vector2f& position = gameObject.getPosition();
        sf::Vector2f newPosition = position;
        newPosition.x += elapsed.asMilliseconds() * 3.0f;
        gameObject.setPosition(newPosition);
    }

    ExampleClockedScript::ExampleClockedScript(GameObject& gameObject) 
        : ClockedScript(gameObject) { }
}

