#pragma once

#include <SFML/Graphics.hpp>


namespace null {
    class WindowMetaInfo {
    private:
        static constexpr sf::Event::KeyEvent defaultKeyEvent = {sf::Keyboard::Key::Unknown,
                                                                false,
                                                                false,
                                                                false,
                                                                false};
    public:
        sf::Event::KeyEvent keyEvent = defaultKeyEvent;
        sf::Vector2u windowsSize = {0, 0};
        sf::Vector2f absoluteMouseWorldCoords = {0, 0};

        [[nodiscard]] sf::Event::KeyEvent getKeyEvent() const;

        sf::Vector2f windowSize = {0, 0};
        char enteredChar = '\0';
        bool leftLock = false;
        bool leftKeyPressed = false;

        // left corner position
        sf::Vector2f position = {0, 0};


        void resetKey();
    };
}



