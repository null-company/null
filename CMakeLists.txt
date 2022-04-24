cmake_minimum_required(VERSION 3.10)

project(NULL_GAME VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(BUILD_SHARED_LIBS OFF)

if (WIN32 AND MSVC)
    # https://github.com/protocolbuffers/protobuf/blob/main/cmake/README.md#notes-on-compiler-warnings
    # applied globally because that's just easier
    add_compile_options(/wd4251)
endif()

include(cmake/libraries.txt)

add_subdirectory(./src/)
add_subdirectory(./test/)

add_executable(NULL_GAME main.cpp)

target_link_libraries(NULL_GAME
        scripts
        nullGameEngine
        sfml-network
        network
        yaml-cpp
        serialization
        sfml-graphics
        sfml-audio
        sfml-window
        sfml-system
        )


