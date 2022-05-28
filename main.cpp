#include <string>

#include <SceneLoader.hpp>
#include <MainLoop.hpp>
#include <iostream>
#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>

#ifdef __linux
#include <X11/Xlib.h>
#endif //__linux

int main() {
    // Uncomment to see logs. Know that it slows execution time
//    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
//    plog::init(plog::debug, &consoleAppender);

    // try /menu or /network-demo-(client|connector|server) or /demo
    std::string levelToLoad;
    bool sceneIsLoaded = false;
    const std::string defaultLevel = "/network-demo-client";
    std::cout << "Type level uri to load.\n"
                 "Empty string for default /network-demo-client.\n"
                 "/network-demo-server to create headless server\n"
                 "Try /demo for single player." << std::endl;
    while (!sceneIsLoaded) {
        std::getline(std::cin, levelToLoad);
        if (levelToLoad.empty()) {
            levelToLoad = defaultLevel;
        }
        try {
            null::SceneLoader::loadSceneFromFile(levelToLoad);
            sceneIsLoaded = true;
        } catch (const null::UnknownSceneException& ignored) {
            std::cout << "Unknown scene, try again" << std::endl;
        }
    }
    null::MainLoop::run();
}
