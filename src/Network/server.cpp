#include <server/ServerArbiter.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <client/Client.h>

int main() {
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    ServerArbiter serverArbiter;
    serverArbiter.listen("127.0.0.1", 5000);
    serverArbiter.launch();

    while (true) {
        std::string oper;
        std::cin >> oper;
        if (oper == "exit") {
            break;
        }
    }
}