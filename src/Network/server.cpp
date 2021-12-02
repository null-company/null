#include <server/ServerArbiter.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <client/Client.h>

int main() {
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    ServerArbiter serverArbiter("127.0.0.1", 4005);
    serverArbiter.launch();
    sf::TcpListener listener;
    sf::Socket::Status result = listener.listen(4006);
    std::cout << (result == sf::Socket::Done);
    while (true) {
        int oper;
        std::cin >> oper;
    }
}