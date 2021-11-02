cmake_minimum_required(VERSION 3.10)

project(network VERSION 1.0 LANGUAGES CXX)
add_executable(network main.cpp src/Client.cpp include/Client.h src/Server.cpp include/Server.h include/Message.h src/message/Message.cpp include/message/Message.h src/channel/Channel.cpp include/channel/Channel.h)

target_link_libraries(network
        sfml-network
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)