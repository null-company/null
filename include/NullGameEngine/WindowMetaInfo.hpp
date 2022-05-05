#pragma once

#include <SFML/Graphics.hpp>


namespace null {
    class WindowMetaInfo {
    private:
        constexpr const static sf::Event::KeyEvent defaultKeyEvent = {sf::Keyboard::Key::Unknown,
                                                                      false,
                                                                      false,
                                                                      false,
                                                                      false};
    public:
        sf::Event::KeyEvent keyEvent = defaultKeyEvent;
        sf::Vector2u windowsSize = {0, 0};
        sf::Vector2f absoluteMouseWorldCoords = {0, 0};

        sf::Event::KeyEvent getKeyEvent();

        void resetKey() ;
    };
}



