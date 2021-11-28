#include <ServerArbiter.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <Client.h>

int main(){
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    Client client("127.0.0.1", 4005);
}