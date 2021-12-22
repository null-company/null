#include <string>

#include <SceneLoader.hpp>
#include <MainLoop.hpp>

// Have to do this on linux
#ifdef __linux
#include <X11/Xlib.h>
#endif //__linux

int main() {
    #ifdef __linux
    XInitThreads();
    #endif //__linux

    std::string mainMenu = "/menu";

    null::SceneLoader::loadSceneFromFile(mainMenu);

    return null::MainLoop::run();
}

