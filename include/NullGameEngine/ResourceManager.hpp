#pragma once

#include <map>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

namespace null {
    class ResourceManager {
    private:
        static std::map<std::string, sf::Texture*> textureCache;
    public:
        static sf::Texture* loadTexture(const std::string& textureName);

        /**
         * It is caller's responsibility to manage returned resource
         * @param soundName name of the sound to load
         * @return pointer to soundBuffer resource
         */
        static sf::SoundBuffer* loadSoundBuffer(const std::string& soundName);

        /**
         * It is caller's responsibility to manage returned resource
         * @param musicName name of the music to load
         * @return
         */
        static sf::Music* openMusic(const std::string& musicName);

        static std::string getTexturePath(const sf::Texture* p_texture);
    };
}
