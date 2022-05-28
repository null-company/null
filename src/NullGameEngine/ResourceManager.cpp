#include <SFML/Graphics.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <ResourceManager.hpp>

#include <filesystem>

namespace null {

    namespace {
        const auto RESOURCES_PATH = std::filesystem::path("../res");
    }

    std::map<std::string, sf::Texture*> ResourceManager::textureCache =
            std::map<std::string, sf::Texture*>();

    sf::Texture* ResourceManager::loadTexture(const std::string& textureName) {
        sf::Texture* texture;
        try {
            texture = ResourceManager::textureCache.at(textureName);
        } catch (std::out_of_range& exception) {
            texture = new sf::Texture();
            constexpr auto texturesDir = "textures";
            texture->loadFromFile((RESOURCES_PATH / texturesDir / textureName).string());
            ResourceManager::textureCache[textureName] = texture;
        }
        return texture;
    }

    std::string ResourceManager::getTexturePath(const sf::Texture *p_texture) {
        for (auto& i : textureCache) {
            if (i.second == p_texture) {
                return i.first;
            }
        }
        return "";
    }

    sf::SoundBuffer* ResourceManager::loadSoundBuffer(const std::string& soundName) {
        // this implementation does not cache because sounds so far are not really used widely
        // and are not repeated like textures
        auto soundBuffer = new sf::SoundBuffer;
        constexpr auto soundsDir = "sounds";
        if (!soundBuffer->loadFromFile((RESOURCES_PATH / soundsDir / soundName).string())) {
            throw std::runtime_error("Sound file not found");
        }

        return soundBuffer;
    }

    sf::Music* ResourceManager::openMusic(const std::string& musicName) {
        auto music = new sf::Music;
        constexpr auto musicDir = "music";
        const auto musicPath = RESOURCES_PATH / musicDir / musicName;
        if (!music->openFromFile(musicPath.string())) {
            throw std::runtime_error("Music file not found");
        }
        return music;
    }

}
