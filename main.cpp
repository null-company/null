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
//    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
//    plog::init(plog::debug, &consoleAppender);

    // try /menu or /network-demo-(client|connector|server) or /demo
    std::string levelToLoad;
    bool sceneIsLoaded = false;
    std::cout << "Type level uri to load" << std::endl;
    while (!sceneIsLoaded) {
        std::cin >> levelToLoad;
        try {
            null::SceneLoader::loadSceneFromFile(levelToLoad);
            sceneIsLoaded = true;
        } catch (const null::UnknownSceneException& _) {
            std::cout << "Unknown scene, try again" << std::endl;
        }
    }
    int port = 5002;
    if (null::MainLoop::isServer && levelToLoad != "/network-demo-connector") {
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
