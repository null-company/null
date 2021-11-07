#include <string>

#include <SceneLoader.hpp>
#include <MainLoop.hpp>

int main() {

    std::string mainMenu = "dummy string";

    null::SceneLoader::loadSceneFromFile(mainMenu);

    return null::MainLoop::run();
}
