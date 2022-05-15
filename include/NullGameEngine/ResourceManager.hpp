#pragma once

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

namespace null {
    class ResourceManager {
    private:
        static std::map<std::string, sf::Texture *> textureCache;
    public:
        static sf::Texture *loadTexture(const std::string &textureName);
    };
}

