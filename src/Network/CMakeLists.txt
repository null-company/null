cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Protobuf REQUIRED)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/Network/)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Network/)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# http://www.cmake.org/Wiki/CMake_FAQ#How_can_I_build_my_MSVC_application_with_a_static_runtime.3F
if(MSVC AND protobuf_MSVC_STATIC_RUNTIME)
    foreach(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif(${flag_var} MATCHES "/MD")
    endforeach()
endif()

set(PROTO_SRC
        ${SRCROOT}/serialized/serverConfig.proto
        )

set(SRC
        ${INCROOT}/client/Client.h
        ${SRCROOT}/client/Client.cpp
        ${INCROOT}/server/GameServer.h
        ${SRCROOT}/server/GameServer.cpp
        ${SRCROOT}/server/ServerArbiter.cpp
        ${SRCROOT}/server/NetClientCollector.cpp
        ${SRCROOT}/utils/NetMessageTransmitting.cpp
        ${INCROOT}/utils/NetMessageTransmitting.h
        ${INCROOT}/utils/util.h
        ${SRCROOT}/utils/util.cpp
        ${INCROOT}/network.h
        ${SRCROOT}/exceptions/NetworkException.cpp
        ${INCROOT}/exceptions/NetworkException.h
        ${SRCROOT}/client/ClientNetworkManager.cpp
        ${INCROOT}/client/ClientNetworkManager.h)
#SET_SOURCE_FILES_PROPERTIES(${PROTO_SRCS} ${PROTO_HDRS} PROPERTIES GENERATED TRUE)

add_library(proto-stuff STATIC ${PROTO_SRC})
target_include_directories(proto-stuff
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
        )
protobuf_generate(TARGET proto-stuff)
target_link_libraries(proto-stuff
        protobuf::libprotobuf
        )

add_library(network STATIC ${SRC})

target_include_directories(network
        PUBLIC ${PROJECT_SOURCE_DIR}/include/Network
        PUBLIC ${PROJECT_SOURCE_DIR}/include/Network/serialized # Done for Protocol Buffer support
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
        PRIVATE ${PROJECT_SOURCE_DIR}/src/Network
        )

target_link_libraries(network
        proto-stuff
        plog
        sfml-graphics
        sfml-audio
        sfml-network
        sfml-window
        sfml-system
        )
