#include <server/ServerArbiter.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <SceneLoader.hpp>
#include <MainLoop.hpp>

int main() {
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    std::string levelToLoad = "/network-demo-server";
//    bool sceneIsLoaded = false;
//    std::cout << "Type level uri to load" << std::endl;
//    while (!sceneIsLoaded) {
//        std::cin >> levelToLoad;
        try {
            null::SceneLoader::loadSceneFromFile(levelToLoad);
//            sceneIsLoaded = true;
        } catch (const null::UnknownSceneException& e) {
            std::cout << "Unknown scene, try again" << std::endl;
        }
//    }

    null::MainLoop::serverArbiter = new ServerArbiter([]() {
        null::MainLoop::isServer = true;
        null::MainLoop::isNetworkingEnabled = true;
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
