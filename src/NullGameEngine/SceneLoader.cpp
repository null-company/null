#include <memory>
        
#include <Scene.hpp>
#include <SceneLoader.hpp>
#include <MainLoop.hpp>

namespace null {

    // todo this is a dummy implementation
    void SceneLoader::loadSceneFromFile(std::string) {
        auto newScene = std::make_unique<Scene>();
        MainLoop::provideScene(move(newScene));
    };

}

