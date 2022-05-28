#include <MusicManager.hpp>

#include <ResourceManager.hpp>
#include <Script.hpp>
#include <GameObject.hpp>

namespace null {

    void MusicManager::start() {
        music = std::unique_ptr<sf::Music>(ResourceManager::openMusic(musicNameToLoad));
        music->setVolume(50);
        music->setLoop(true);
        music->play();
    }

    void MusicManager::update() {
    }

    MusicManager::MusicManager(GameObject& gameObject) : Script(gameObject) { }
}
