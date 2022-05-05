cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/NullGameEngine/)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/NullGameEngine/)

set(SRC
        ${INCROOT}/NullGameEngine.hpp
        ${SRCROOT}/GameObject.cpp
        ${INCROOT}/GameObject.hpp
        ${INCROOT}/Script.hpp
        ${SRCROOT}/Component.cpp
        ${INCROOT}/Component.hpp
        ${SRCROOT}/Scene.cpp
        ${INCROOT}/Scene.hpp
        ${SRCROOT}/SceneLoader.cpp
        ${INCROOT}/SceneLoader.hpp
        ${SRCROOT}/MainLoop.cpp
        ${INCROOT}/MainLoop.hpp
        ${SRCROOT}/Camera.cpp
        ${INCROOT}/Camera.hpp
        ${SRCROOT}/Renderer.cpp
        ${INCROOT}/Renderer.hpp
        ${SRCROOT}/ResourceManager.cpp
        ${INCROOT}/ResourceManager.hpp
        ${INCROOT}/RenderLayer.hpp
        ${INCROOT}/SpriteSheet.hpp
        ${SRCROOT}/SpriteSheet.cpp
        ${SRCROOT}/Animation.cpp
        ${INCROOT}/Animation.hpp
        ${SRCROOT}/RigidBodyAnimation.cpp
        ${INCROOT}/RigidBodyAnimation.hpp
        ${SRCROOT}/Utility.cpp
        ${INCROOT}/Utility.hpp
        ${SRCROOT}/WindowMetaInfo.cpp
        ${INCROOT}/WindowMetaInfo.hpp
        ${SRCROOT}/GameObjectManagers/MapManager.cpp
        ${INCROOT}/MapManager/MapManager.hpp
        ${SRCROOT}/Movement/Movable.cpp
        ${SRCROOT}/Movement/LinearMovable.cpp
        ${INCROOT}/Movememt/LinearMovable.hpp Physics/ContactListener.cpp ../../include/NullGameEngine/Physics/ContactListener.hpp ../../include/Scripts/Weapons/WeaponHolders/GrenadeBunchScript.hpp ../../include/NullGameEngine/Physics/CollisionCategories.hpp)

add_library(nullGameEngine STATIC ${SRC})
target_include_directories(nullGameEngine
        PUBLIC ${PROJECT_SOURCE_DIR}/include/NullGameEngine
        PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(nullGameEngine
        box2d
        scripts
        serialization
        network
        sfml-graphics
        sfml-audio
        sfml-network
        sfml-window
        sfml-system
        )


