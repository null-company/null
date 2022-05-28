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

        [[nodiscard]]
        sf::Event::KeyEvent getKeyEvent() const;

        void resetKey();
    };
}



