cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/Scripts/)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Scripts/)

set(SRC
    ${INCROOT}/Scripts.hpp # declares all public scripts
    ${SRCROOT}/ExampleScript.cpp
    ${INCROOT}/ExampleScript.hpp
    ${SRCROOT}/ClockedScript.cpp
    ${INCROOT}/ClockedScript.hpp
    ${SRCROOT}/ExampleClockedScript.cpp
    ${INCROOT}/ExampleClockedScript.hpp
    ${SRCROOT}/ReloadSceneScript.cpp
    ${INCROOT}/ReloadSceneScript.hpp
    ${SRCROOT}/CameraScript.cpp
    ${INCROOT}/CameraScript.hpp
    ${SRCROOT}/ExampleCameraScript.cpp
    ${INCROOT}/ExampleCameraScript.hpp
    )

add_library(scripts STATIC ${SRC})
target_include_directories(scripts
    PUBLIC ${PROJECT_SOURCE_DIR}/include/Scripts
    PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(scripts
    nullGameEngine
    sfml-graphics
    sfml-audio
    sfml-network
    sfml-window
    sfml-system
    )


