cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/NullGameEngine/)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/NullGameEngine/)

set(SRC
    ${SRCROOT}/NullGameEngine.cpp
    ${INCROOT}/NullGameEngine.hpp
    ${SRCROOT}/GameObject.cpp
    ${INCROOT}/GameObject.hpp
    ${SRCROOT}/Script.cpp
    ${INCROOT}/Script.hpp
    ${SRCROOT}/Component.cpp
    ${INCROOT}/Component.hpp
    ${SRCROOT}/Scene.cpp
    ${INCROOT}/Scene.hpp
    ${SRCROOT}/SceneLoader.cpp
    ${INCROOT}/SceneLoader.hpp
    ${SRCROOT}/MainLoop.cpp
    ${INCROOT}/MainLoop.hpp
    )

add_library(nullGameEngine STATIC ${SRC})
target_include_directories(nullGameEngine 
    PUBLIC ${PROJECT_SOURCE_DIR}/include/NullGameEngine
    PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(nullGameEngine sfml-graphics
  sfml-audio
  sfml-network
  sfml-window
  sfml-system
  )


