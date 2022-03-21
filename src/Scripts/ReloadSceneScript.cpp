#include <ReloadSceneScript.hpp>

#include <MainLoop.hpp>
#include <SceneLoader.hpp>

namespace null {

    ReloadSceneScript::ReloadSceneScript(GameObject& gameObject)
            : Script(gameObject) {}

    void ReloadSceneScript::start() {

    }

    void ReloadSceneScript::update() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            SceneLoader::changeScene("todo example");
        }
    }
}
