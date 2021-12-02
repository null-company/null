#include <string>

#include <SceneLoader.hpp>
#include <MainLoop.hpp>
#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>

// Have to do this on linux
#ifdef __linux
#include <X11/Xlib.h>
#endif //__linux

int main() {

    // Init logging
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);

    #ifdef __linux
    XInitThreads();
    #endif //__linux


    std::string mainMenu = "dummy string";

    null::SceneLoader::loadSceneFromFile(mainMenu);

    return  null::MainLoop::run();
}

