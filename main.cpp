#include <string>

#include <SceneLoader.hpp>
#include <MainLoop.hpp>
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

#ifdef __linux
    XInitThreads();
#endif //__linux

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
        null::MainLoop::serverArbiter = new ServerArbiter(null::MainLoop::run);
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
        null::MainLoop::clientNetworkManager = new ClientNetworkManager("127.0.0.1", 5000);


    }

    return null::MainLoop::run();
}
