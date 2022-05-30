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
        static std::map<std::string, std::shared_ptr<sf::Music>> musicPool;
        static std::map<std::string, sf::Sound> soundPool;
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
        static std::shared_ptr<sf::Music> openMusic(const std::string& musicName);

        /**
         * Get a globally available sound. Playing it never dies
         * Note that concurrent plays won't work
         * @param soundName
         * @return
         */
        static sf::Sound& getSound(const std::string& soundName);

        static std::string getTexturePath(const sf::Texture* p_texture);
    };
}
