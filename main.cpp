#include <string>

#include <SceneLoader.hpp>
#include <MainLoop.hpp>
#include <iostream>
#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <server/ServerArbiter.h>

#ifdef __linux
#include <X11/Xlib.h>
#endif //__linux

int main() {
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);

    // try /menu or /network-demo-(client|connector|server) or /demo
    std::string levelToLoad;
    bool sceneIsLoaded = false;
    std::cout << "Type level uri to load" << std::endl;
    while (!sceneIsLoaded) {
        std::cin >> levelToLoad;
        try {
            null::SceneLoader::loadSceneFromFile(levelToLoad);
            sceneIsLoaded = true;
        } catch (const null::UnknownSceneException& ignored) {
            std::cout << "Unknown scene, try again" << std::endl;
        }
    }
    null::MainLoop::run();
}
