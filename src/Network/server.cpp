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
    while (!sceneIsLoaded) {
        std::cin >> levelToLoad;
        try {
            null::SceneLoader::loadSceneFromFile(levelToLoad);
            sceneIsLoaded = true;
        } catch (const null::UnknownSceneException& e) {
            std::cout << "Unknown scene, try again" << std::endl;
        }
    }
    srand(time(NULL));
    int port = 5002;
    if (null::MainLoop::isServer and levelToLoad != "/network-demo-connector") {
        LOGD << "This is a server";
        null::MainLoop::serverArbiter = new ServerArbiter([]() { null::MainLoop::run(true); });
        auto& serverArbiter = *null::MainLoop::serverArbiter;
        serverArbiter.listen("127.0.0.1", port);
        serverArbiter.launch();
        while (true) {
            std::string oper;
            std::cin >> oper;
            if (oper == "exit") {
                break;
            }
        }
    } else if (levelToLoad == "/network-demo-connector") {
        LOGD << "This is a client that wants to connect";
        null::MainLoop::clientNetworkManager = new ClientNetworkManager("127.0.0.1", port);
        auto& clientNetworkManager = *null::MainLoop::clientNetworkManager;
        clientNetworkManager.getClient().connectRoom("AAAAAA");
//        LOGD << clientNetworkManager.getClient().getRoomCode();
        null::MainLoop::run();
    } else {
        LOGD << "This is a client";
        null::MainLoop::clientNetworkManager = new ClientNetworkManager("127.0.0.1", port);
        auto& clientNetworkManager = *null::MainLoop::clientNetworkManager;
        clientNetworkManager.getClient().createRoom();
        LOGD << clientNetworkManager.getClient().getRoomCode();
        null::MainLoop::run();
    }
}