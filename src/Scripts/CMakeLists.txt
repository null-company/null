cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
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
        ${SRCROOT}/CursorAnimation.cpp
        ${INCROOT}/CursorAnimation.hpp
        ${SRCROOT}/PlayerAnimation.cpp
        ${INCROOT}/PlayerAnimation.hpp
        ${SRCROOT}/ButtonScript.cpp
        ${INCROOT}/ButtonScript.hpp
        ${SRCROOT}/NetworkPlayerScript.cpp
        ${SRCROOT}/Weapons/WeaponHolders/WeaponScript.cpp
        ${SRCROOT}/Weapons/WeaponHolders/StraightWeaponScript.cpp
        ${INCROOT}/Weapons/WeaponHolders/StraightWeaponScript.hpp
        ${INCROOT}/Weapons/WeaponHolders/WeaponScript.hpp
        ${SRCROOT}/Weapons/WeaponAmmunition/BulletScript.cpp
        ${INCROOT}/Weapons/WeaponAmmunition/BulletScript.hpp
        ${SRCROOT}/Weapons/WeaponHolders/GrenadeBunchScript.cpp
        ${INCROOT}/Weapons/WeaponAmmunition/GrenadeScript.hpp
        ${SRCROOT}/Weapons/WeaponAmmunition/GrenadeScript.cpp
        ${INCROOT}/Weapons/WeaponAmmunition/GrenadeScript.hpp
        ${SRCROOT}/Weapons/WeaponHolders/WeaponStorage.cpp
        ${INCROOT}/Weapons/WeaponHolders/WeaponStorage.hpp
        ${SRCROOT}/Weapons/WeaponGenerator.cpp
        ${INCROOT}/Weapons/WeaponGenerator.hpp
        ${SRCROOT}/GameObjectSensor.cpp
        ${INCROOT}/GameObjectSensor.hpp
        ${SRCROOT}/DeleteScript.cpp
        ${INCROOT}/DeleteScript.hpp
        ${SRCROOT}/PlayerControlledBox/PlayerControlledBoxClient.cpp
        ${INCROOT}/PlayerControlledBox/PlayerControlledBoxClient.hpp
        ${SRCROOT}/PlayerControlledBox/PlayerControlledBoxServer.cpp
        ${INCROOT}/PlayerControlledBox/PlayerControlledBoxServer.hpp
        ${SRCROOT}/Network/NetworkManagerClientScript.cpp
        ${INCROOT}/Network/NetworkManagerClientScript.hpp
        ${SRCROOT}/Network/NetworkManagerServerScript.cpp
        ${INCROOT}/Network/NetworkManagerServerScript.hpp
        ${SRCROOT}/Network/PrimitiveStateConverter.cpp
        ${INCROOT}/Network/PrimitiveStateConverter.hpp
        ${SRCROOT}/Network/Utility/LastAcceptedMessageProcessor.cpp
        ${INCROOT}/Network/Utility/LastAcceptedMessageProcessor.hpp
        ${SRCROOT}/Network/Utility/EveryAcceptedMessageProcessor.cpp
        ${INCROOT}/Network/Utility/EveryAcceptedMessageProcessor.hpp
        ${SRCROOT}/MusicManager.cpp
        ${INCROOT}/MusicManager.hpp
        ${SRCROOT}/PlayerProgress/HealthBarHolder.cpp
        ${INCROOT}/PlayerProgress/HealthBarHolder.hpp
        ${SRCROOT}/PlayerProgress/HealthBar.cpp
        ${INCROOT}/PlayerProgress/HealthBar.hpp
        ${INCROOT}/Wall.hpp
        ${SRCROOT}/Wall.cpp
        ${INCROOT}/Network/PlayerDispatchers/PlayerDispatcherClient.hpp
        ${SRCROOT}/Network/PlayerDispatchers/PlayerDispatcherClient.cpp
        ${INCROOT}/Network/PlayerDispatchers/PlayerDispatcherServer.hpp
        ${SRCROOT}/Network/PlayerDispatchers/PlayerDispatcherServer.cpp
        ${SRCROOT}/TextHandler.cpp
        ${INCROOT}/TextHandler.hpp
        ${SRCROOT}/BackgroundAnimation.cpp
        ${INCROOT}/BackgroundAnimation.hpp)


add_library(scripts STATIC ${SRC})
target_include_directories(scripts
        PUBLIC ${PROJECT_SOURCE_DIR}/include/Scripts
        PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(scripts
        nullGameEngine
        serialization
        network
        utils
        sfml-graphics
        sfml-audio
        sfml-network
        sfml-window
        sfml-system
        )
