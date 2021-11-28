cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(${INCROOT}/serialized/NetMessage.h ${SRCROOT}/serialized/NetMessage.cpp serialized/serverConfig.proto)
set(INCROOT ${PROJECT_SOURCE_DIR}/include/Network/)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Network/)
set(SRC
        ${INCROOT}/Client.h
        ${SRCROOT}/Client.cpp
        ${INCROOT}/server/Server.h
        ${SRCROOT}/server/Server.cpp
        ${INCROOT}/channel/Connection.h
        ${SRCROOT}/channel/Connection.cpp
        ${INCROOT}/serialized/serverConfig.pb.h
        ${SRCROOT}/serialized/serverConfig.pb.cc
        server/ServerArbiter.cpp server/ClientCollector.cpp NetMessageTransmitting.cpp)
add_library(network STATIC ${SRC})
target_link_libraries(network ${Protobuf_LIBRARIES})
target_include_directories(network
        PUBLIC ${PROJECT_SOURCE_DIR}/include/Network
        PUBLIC ${PROJECT_SOURCE_DIR}/include/Network/serialized # Done for Protocol Buffer support
        PRIVATE ${PROJECT_SOURCE_DIR}/src/Network)
add_executable(NETWORK main.cpp)

target_link_libraries(NETWORK
        network
        )

target_link_libraries(network

        plog
        sfml-graphics
        sfml-audio
        sfml-network
        sfml-window
        sfml-system
        )


