#include <SFML/System.hpp>

#include <Script.hpp>
#include <GameObject.hpp>
#include <ExampleScript.hpp>

namespace null {

    sf::Clock clock;

    void ExampleScript::start() {
        clock.restart();
    }

    void ExampleScript::update() {
        // todo that kind of clock management is 
        // basically boilerplate code. could be done in the
        // Script class itself or in the component or something
        // else if it's smart enough
        sf::Time elapsed = clock.restart();

        const sf::Vector2f& position = gameObject.getPosition();
        sf::Vector2f newPosition = position;
        newPosition.x += elapsed.asMilliseconds() * 3.0f;
        gameObject.setPosition(newPosition);
    }

    ExampleScript::ExampleScript(GameObject& gameObject) : Script(gameObject) {  }

}

