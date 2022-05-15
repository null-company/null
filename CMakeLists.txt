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
message(STATUS "SFML downloaded")

FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v2.4.1
)
message(STATUS "Started downloading box2d")
FetchContent_MakeAvailable(box2d)
message(STATUS "box2d downloaded")


add_subdirectory(./src/)
add_subdirectory(./test/)

add_executable(NULL_GAME main.cpp)

add_executable(NULL_GAME_SERVER server.cpp)

target_link_libraries(NULL_GAME
        scripts
        nullGameEngine
        sfml-network
        network
        serialization
        sfml-graphics
        sfml-audio
        sfml-window
        sfml-system
        sfml-network
        )


