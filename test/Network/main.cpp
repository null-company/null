//#include <gtest/gtest.h>
#include <iostream>
#include <gtest/gtest.h>
#include "plog/Init.h"
#include "plog/Appenders/ConsoleAppender.h"
#include "plog/Formatters/TxtFormatter.h"
#include "server/ServerArbiter.h"
#include "client/ClientNetworkManagerTest.h"

int main(int argc, char ** argv){
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

