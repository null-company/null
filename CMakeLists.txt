cmake_minimum_required(VERSION 3.10)

project(NULL_GAME VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
 SFML
 GIT_REPOSITORY https://github.com/SFML/SFML.git
 GIT_TAG 2.5.1
)
FetchContent_MakeAvailable(SFML)

add_executable(NULL_GAME main.cpp)

target_link_libraries(NULL_GAME sfml-graphics
  sfml-audio
  sfml-network
  sfml-window
  sfml-system
  )


