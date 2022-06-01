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

int main(int argc, char** argv) {
    // Uncomment to see logs. Know that it slows execution time
//    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
//    plog::init(plog::debug, &consoleAppender);

    if (argc > 2) {
        std::cerr << "Provide 1 arg to load exact scene or no args to interact" << std::endl;
        exit(424242);
    }

    if (argc == 2) {
        std::cout << "You chose " << argv[1] << std::endl;
        std::string levelToLoad = argv[1];
        try {
            null::SceneLoader::loadSceneFromFile(levelToLoad);
        } catch (const null::UnknownSceneException& ignored) {
            std::cerr << "Unknown scene, try again" << std::endl;
            exit(30);
        }
        return null::MainLoop::run();
    }

    // try /menu or /network-demo-(client|connector|server) or /demo
    std::string levelToLoad;
    bool sceneIsLoaded = false;
    const std::string defaultLevel = "/network-demo-client";
    std::cout << "Type level uri to load.\n"
                 "Empty string for default /network-demo-client.\n"
                 //                 "/network-demo-server to create headless server\n"
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
            std::cerr << "Unknown scene, try again" << std::endl;
        }
    }
    null::MainLoop::run();
}
