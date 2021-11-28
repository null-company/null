#include <string>

#include <SceneLoader.hpp>
#include <MainLoop.hpp>
#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>

int main() {
    // Init logging
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);

    std::string mainMenu = "dummy string";

    null::SceneLoader::loadSceneFromFile(mainMenu);

    return null::MainLoop::run();
}
