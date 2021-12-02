#include <network.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>

int main() {
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    Client client("127.0.0.1", 4005);
    while (true) {
        int oper;
        std::cin >> oper;
    }
}