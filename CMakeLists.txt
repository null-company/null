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
message(STATUS "Started downloading SFML")
FetchContent_MakeAvailable(SFML)
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

add_executable(NULL_GAME main.cpp include/NullGameEngine/RenderLayer.hpp)

target_link_libraries(NULL_GAME
    nullGameEngine
    scripts
    sfml-graphics
    sfml-audio
    sfml-network
    sfml-window
    sfml-system
    )


