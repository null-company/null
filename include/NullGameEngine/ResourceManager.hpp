#ifndef NULL_GAME_RESOURCEMANAGER_HPP
#define NULL_GAME_RESOURCEMANAGER_HPP
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


#endif //NULL_GAME_RESOURCEMANAGER_HPP
