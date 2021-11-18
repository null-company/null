#pragma once

#include <map>
#include <string>

namespace  null {
    class ResourceManager {
    private:
        static std::map<std::string, sf::Texture*> textureMap;
    public:
        static sf::Texture* loadTexture(const std::string &filename);
    };
}
