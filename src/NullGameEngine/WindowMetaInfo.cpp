

#include "WindowMetaInfo.hpp"

namespace null {

    void WindowMetaInfo::resetKey() {
        this->keyEvent = defaultKeyEvent;
    }

    sf::Event::KeyEvent WindowMetaInfo::getKeyEvent() const {
        return keyEvent;
    }
}