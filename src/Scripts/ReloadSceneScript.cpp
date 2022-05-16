#include <ReloadSceneScript.hpp>

#include <MainLoop.hpp>
#include <SceneLoader.hpp>
#include "Serializer.hpp"

namespace null {

    ReloadSceneScript::ReloadSceneScript(GameObject& gameObject)
            : Script(gameObject) {}

    void ReloadSceneScript::start() {

    }

    void ReloadSceneScript::update() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            SceneLoader::changeScene("/demo");
        }
    }

    void ReloadSceneScript::serialize(google::protobuf::Message& message) const {
        auto& msg = dynamic_cast<serial::Script&>(message);
        msg.mutable_reload_scene_script();
    }

    std::unique_ptr<Component> ReloadSceneScript::deserialize(const google::protobuf::Message& message) {
        auto& msg = dynamic_cast<const serial::Script&>(message);
        return std::make_unique<ReloadSceneScript>(
                *Serializer::currentDeserializationGameObject
                );
    }
}
