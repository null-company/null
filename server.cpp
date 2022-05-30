#include <network.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <SceneLoader.hpp>
#include <MainLoop.hpp>
#include <iostream>

int main(int argc, char** argv) {
    // Uncomment to see logs. Know that it slows execution time
//    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
//    plog::init(plog::debug, &consoleAppender);

    if (argc > 2) {
        std::cerr << "Provide 1 arg to load exact scene or no args to interact" << std::endl;
        exit(424242);
    }

    null::MainLoop::serverArbiter = new ServerArbiter([]() {
        null::MainLoop::attachWindow = true;
        null::MainLoop::run();
    });
    null::MainLoop::serverArbiter->listen(5002);

    if (argc == 2) {
        std::cout << "You chose" << argv[1] << std::endl;
        std::string levelToLoad = argv[1];
        try {
            null::SceneLoader::loadSceneFromFile(levelToLoad);
        } catch (const null::UnknownSceneException& ignored) {
            std::cerr << "Unknown scene, try again" << std::endl;
            exit(30);
        }
        std::cout << "Server is up. Type 'exit' to stop." << std::endl;
        null::MainLoop::serverArbiter->launch();
        while (true) {
            std::string oper;
            std::cin >> oper;
            if (oper == "exit") {
                break;
            }
        }
    }

    const std::string defaultLevel = "/network-demo-server";
    std::string levelToLoad;
    bool sceneIsLoaded = false;
    std::cout << "Type level uri to load. Empty string for default /network-demo-server" << std::endl;
    while (!sceneIsLoaded) {
        std::getline(std::cin, levelToLoad);
        if (levelToLoad.empty()) {
            levelToLoad = defaultLevel;
        }
        try {
            null::SceneLoader::loadSceneFromFile(levelToLoad);
            sceneIsLoaded = true;
        } catch (const null::UnknownSceneException& e) {
            std::cout << "Unknown scene, try again" << std::endl;
        }
    }

    null::MainLoop::serverArbiter->launch();
    std::cout << "Server is up. Type 'exit' to stop." << std::endl;
    while (true) {
        std::string oper;
        std::cin >> oper;
        if (oper == "exit") {
            break;
        }
    }
}
