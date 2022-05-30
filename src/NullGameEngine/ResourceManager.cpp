#include <SFML/Graphics.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <ResourceManager.hpp>

#include <filesystem>

namespace null {

    namespace {
        const auto RESOURCES_PATH = std::filesystem::path("../res");
    }

    std::map<std::string, sf::Texture*> ResourceManager::textureCache{};
    std::map<std::string, sf::Sound> ResourceManager::soundPool{};
    std::map<std::string, std::shared_ptr<sf::Music>> ResourceManager::musicPool{};
    std::map<std::string, std::shared_ptr<sf::Font>> ResourceManager::fontPool{};

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

    std::shared_ptr<sf::Font> ResourceManager::loadFont(const std::string& fontName) {
        std::shared_ptr<sf::Font> font = std::make_shared<sf::Font>();
        try {
            font = ResourceManager::fontPool.at(fontName);
        } catch (std::out_of_range& exception) {
            constexpr auto fontsDir = "fonts";
            if (!font->loadFromFile(RESOURCES_PATH / fontsDir / fontName)) {
                throw std::runtime_error("Font file not found");
            };
            ResourceManager::fontPool[fontName] = font;
        }
        return font;
    }

    std::string ResourceManager::getTexturePath(const sf::Texture* p_texture) {
        for (auto& i: textureCache) {
            if (i.second == p_texture) {
                return i.first;
            }
        }
        return "";
    }

    sf::SoundBuffer* ResourceManager::loadSoundBuffer(const std::string& soundName) {
        static std::map<std::string, sf::SoundBuffer> cache;
        if (auto r = cache.find(soundName); r != cache.end()) {
            return &r->second;
        }
        // this implementation does not cache because sounds so far are not really used widely
        // and are not repeated like textures
//        auto soundBuffer = new sf::SoundBuffer;
        auto soundBuffer = &cache[soundName];
        constexpr auto soundsDir = "sounds";
        if (!soundBuffer->loadFromFile((RESOURCES_PATH / soundsDir / soundName).string())) {
            throw std::runtime_error("Sound file not found");
        }

        return soundBuffer;
    }

    std::shared_ptr<sf::Music> ResourceManager::openMusic(const std::string& musicName) {
        if (auto r = musicPool.find(musicName); r != musicPool.end()) {
            return r->second; // return cached instance
        }
        musicPool[musicName] = std::make_shared<sf::Music>();
        auto& newInstance = musicPool[musicName];
        constexpr auto musicDir = "music";
        const auto musicPath = RESOURCES_PATH / musicDir / musicName;
        if (!newInstance->openFromFile(musicPath.string())) {
            throw std::runtime_error("Music file not found");
        }
        return newInstance;
    }

    sf::Sound& ResourceManager::getSound(const std::string& soundName) {
        if (auto r = soundPool.find(soundName); r != soundPool.end()) {
            return r->second; // return cached instance
        }
        soundPool[soundName] = sf::Sound(*loadSoundBuffer(soundName));
        return soundPool[soundName];
    }
}
