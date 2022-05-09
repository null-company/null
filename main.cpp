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

    const std::string ip("127.0.0.1");

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
    if (null::MainLoop::isServer) {
        LOGD << "This is a server";
        null::MainLoop::serverArbiter = new ServerArbiter([]() {
            null::MainLoop::isServer = true; // needs to be set in this thread, because isServer is thread_local
            null::MainLoop::run();
        });
        auto& serverArbiter = *null::MainLoop::serverArbiter;
        uint16_t port;
        std::cout << "Port to connect " << std::flush;
        std::cin >> port;
        std::cout << std::endl;
        serverArbiter.listen("127.0.0.1", port);
        serverArbiter.launch();
        while (true) {
            std::string oper;
            std::cin >> oper;
            if (oper == "exit") {
                break;
            }
        }
    } else {
        std::cout << "Create room?[y/n] " << std::flush;
        std::string createRoom;
        std::cin >> createRoom;
        std::cout << "\n";
        if (createRoom != "y" && createRoom != "n") {
            std::cout << "wrong input" << std::endl;
            return 1;
        }
        uint16_t port;
        std::cout << "Port: " << std::flush;
        std::cin >> port;
        std::cout << std::endl;
        if (createRoom == "y") {
            LOGD << "This is a client which will create a room";
            null::MainLoop::clientNetworkManager = new ClientNetworkManager(ip, port);
            auto& clientNetworkManager = *null::MainLoop::clientNetworkManager;
            clientNetworkManager.getClient().createRoom();
            std::cout << "Room: " << clientNetworkManager.getClient().getRoomCode() << std::endl;
            LOGD << clientNetworkManager.getClient().getRoomCode();
            null::MainLoop::run();
        } else if (createRoom == "n") {
            LOGD << "This is a client that wants to connect";
            null::MainLoop::clientNetworkManager = new ClientNetworkManager(ip, port);
            auto& clientNetworkManager = *null::MainLoop::clientNetworkManager;
            std::cout << "Room code: " << std::flush;
            std::string roomCode;
            std::cin >> roomCode;
            clientNetworkManager.getClient().connectRoom(roomCode);
            null::MainLoop::run();
        }
    }
}
