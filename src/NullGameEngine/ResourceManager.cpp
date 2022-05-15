#include <SFML/Graphics.hpp>
#include <ResourceManager.hpp>

namespace null {
    std::map<std::string, sf::Texture*> ResourceManager::textureCache =
            std::map<std::string, sf::Texture*>();

    sf::Texture* ResourceManager::loadTexture(const std::string& textureName) {
        sf::Texture* texture;
        try {
            texture = ResourceManager::textureCache.at(textureName);
        } catch (std::out_of_range& exception) {
            texture = new sf::Texture();
            texture->loadFromFile("../res/textures/" + textureName);
            ResourceManager::textureCache[textureName] = texture;
        }
        return texture;
    }

    std::string ResourceManager::getTexturePath(const sf::Texture *p_texture) {
        for (auto& i : textureMap) {
            if (i.second == p_texture) {
                return i.first;
            }
        }
        return "";
    }

}

