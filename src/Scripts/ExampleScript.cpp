/* #include <Scripts.hpp> */
#include <GameObject.hpp>
#include <SFML/System.hpp>

namespace null {

    class ExampleScript : public Script {
        sf::Clock clock;
        
        public:
        void start() override {
            clock.restart();
        }

        void update() override {
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

        explicit ExampleScript(GameObject& gameObject) : Script(gameObject) {  }
    };

}

