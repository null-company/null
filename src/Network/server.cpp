#include <server/ServerArbiter.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <SceneLoader.hpp>
#include <MainLoop.hpp>

int main() {
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
//    ServerArbiter serverArbiter;
//    serverArbiter.listen("127.0.0.1", 5000);
//    serverArbiter.launch();

//    while (true) {
//        std::string oper;
//        std::cin >> oper;
//        if (oper == "exit") {
//            break;
//        }
//    }
    std::string levelToLoad; // see /menu or /network-demo or /demo
    bool sceneIsLoaded = false;
    std::cout << "Type level uri to load" << std::endl;
    while(!sceneIsLoaded) {
        std::cin >> levelToLoad;
        try {
            null::SceneLoader::loadSceneFromFile(levelToLoad);
            sceneIsLoaded = true;
        } catch (const null::UnknownSceneException& e) {
            std::cout << "Unknown scene, try again" << std::endl;
        }
    }

    if (null::MainLoop::isServer) {
        LOGD << "This is a server";
        null::MainLoop::serverArbiter = new ServerArbiter([](){ null::MainLoop::run(); });
        auto& serverArbiter = *null::MainLoop::serverArbiter;
        serverArbiter.listen("127.0.0.1", 5000);
        serverArbiter.launch();
        while (true) {
            std::string oper;
            std::cin >> oper;
            if (oper == "exit") {
                break;
            }
        }
    } else {
        LOGD << "This is a client";
        null::MainLoop::clientNetworkManager = new ClientNetworkManager("127.0.0.1", 5000);
        auto& clientNetworkManager = *null::MainLoop::clientNetworkManager;
        clientNetworkManager.getClient().createRoom();
        LOGD << clientNetworkManager.getClient().getRoomCode();
        null::MainLoop::run();
    }
}