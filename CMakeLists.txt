cmake_minimum_required(VERSION 3.11)

project(NULL_GAME VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(NULL_BUILD_TESTS "Build tests for null-game" OFF)

set(BUILD_SHARED_LIBS OFF)

if(WIN32)
    foreach(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif(${flag_var} MATCHES "/MD")
    endforeach()
endif()

if (WIN32 AND MSVC)
    # https://github.com/protocolbuffers/protobuf/blob/main/cmake/README.md#notes-on-compiler-warnings
    # applied globally because that's just easier
    add_compile_options(/wd4251)
endif ()

include(cmake/libraries.cmake)

add_subdirectory(./src/)

if (NULL_BUILD_TESTS)
    add_subdirectory(./test/)
endif ()

add_executable(NULL_GAME main.cpp)

add_executable(NULL_LEVEL_CHOOSER level-chooser.cpp)

add_executable(NULL_GAME_SERVER server.cpp)

target_link_libraries(NULL_GAME
        scripts
        nullGameEngine
        sfml-network
        plog
        network
        serialization
        sfml-graphics
        sfml-audio
        sfml-window
        sfml-system
        sfml-network
        )

target_link_libraries(NULL_GAME_SERVER
        scripts
        nullGameEngine
        sfml-network
        network
        yaml-cpp
        plog
        serialization
        sfml-graphics
        sfml-audio
        sfml-window
        sfml-system
        )

target_link_libraries(NULL_LEVEL_CHOOSER
        scripts
        nullGameEngine
        sfml-network
        plog
        network
        serialization
        sfml-graphics
        sfml-audio
        sfml-window
        sfml-system
        sfml-network
        )
