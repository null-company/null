#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"

namespace null {
    std::map<std::string, sf::Texture*> ResourceManager::textureMap =
            std::map<std::string, sf::Texture*>();

    sf::Texture* ResourceManager::loadTexture(const std::string& filename) {
        auto texture = new sf::Texture();
        try {
            texture = ResourceManager::textureMap.at(filename);
        } catch (std::out_of_range& exception) {
            texture->loadFromFile("../res/textures/" + filename);
            ResourceManager::textureMap[filename] = texture;
        }
        return texture;
    }
}
