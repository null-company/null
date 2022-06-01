#include <iostream>
#include <string>

#include <SceneLoader.hpp>
#include <MainLoop.hpp>

#ifdef __linux
#include <X11/Xlib.h>
#endif //__linux

int main() {
    const std::string defaultLevel = "/menu";
    try {
        null::SceneLoader::loadSceneFromFile(defaultLevel);
    } catch (const null::UnknownSceneException& ignored) {
        std::cerr << "Unknown scene, try again" << std::endl;
        exit(9090);
    }
    return null::MainLoop::run();
}
