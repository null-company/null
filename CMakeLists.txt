cmake_minimum_required(VERSION 3.10)

project(NULL_GAME VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG d950c9387a94087f478434fb42b95c5a9c746fb2 # todo this needs revision. 2.5.1 doesnt fit for clang
)
FetchContent_MakeAvailable(SFML)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif (WIN32)

FetchContent_MakeAvailable(googletest)

add_subdirectory(./src/)
add_subdirectory(./test/)
add_executable(NULL_GAME main.cpp include/Network/Serializable.h)


target_link_libraries(NULL_GAME
        nullGameEngine
        network
        sfml-graphics
        sfml-audio
        sfml-network
        sfml-window
        sfml-system
        )



