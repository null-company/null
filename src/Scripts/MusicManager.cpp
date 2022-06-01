#include <MusicManager.hpp>

#include <ResourceManager.hpp>
#include <Script.hpp>
#include <GameObject.hpp>
#include "Serializer.hpp"

namespace null {

    void MusicManager::start() {
        music = ResourceManager::openMusic(musicNameToLoad);
        music->setVolume(40);
        music->setLoop(true);
        music->play();
    }

    void MusicManager::update() {
    }

    MusicManager::MusicManager(GameObject& gameObject) : Script(gameObject) { }

    MusicManager::~MusicManager() {
        if (music != nullptr) {
            music->stop();
        }
    }

    void MusicManager::serialize(google::protobuf::Message& message) const {
        auto& msg = dynamic_cast<serial::Script&>(message);
        auto s_script = msg.mutable_music_manager();
        s_script->set_musicpath(musicNameToLoad);
    }

    std::unique_ptr<Component> MusicManager::deserialize(const google::protobuf::Message& message) {
        auto& msg = dynamic_cast<const serial::Script&>(message);
        auto const& s_script = msg.music_manager();
        auto p_script =  std::make_unique<MusicManager>(
                *Serializer::currentDeserializationGameObject
        );
        p_script->musicNameToLoad = s_script.musicpath();
        return p_script;
    }

}
