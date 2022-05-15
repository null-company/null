#include <network.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <SceneLoader.hpp>
#include <MainLoop.hpp>
#include <iostream>

int main() {
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
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

    null::MainLoop::serverArbiter = new ServerArbiter([]() {
        null::MainLoop::attachWindow = true;
        null::MainLoop::run();
    });
    null::MainLoop::serverArbiter->listen(5002);
    null::MainLoop::serverArbiter->launch();

    while (true) {
        std::string oper;
        std::cin >> oper;
        if (oper == "exit") {
            break;
        }
    }
}
