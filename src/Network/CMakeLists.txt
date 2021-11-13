cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/Network/)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Network/)
set(SRC
        ${INCROOT}/Client.h
        ${SRCROOT}/Client.cpp
        ${INCROOT}/Server.h
        ${SRCROOT}/Server.cpp
        ${INCROOT}/message/Message.h
        ${SRCROOT}/message/Message.cpp
        ${INCROOT}/channel/Channel.h
        ${SRCROOT}/channel/Channel.cpp
        SnapshotSystem/SnapshotManager.cpp ../../include/Network/SnapshotSystem/SnapshotManager.h SnapshotSystem/Snapshot.cpp ../../include/Network/SnapshotSystem/Snapshot.h SnapshotSystem/SnapshotDelta.cpp ../../include/Network/SnapshotSystem/SnapshotDelta.h SnapshotSystem/SnapshotDelta.cpp)
add_library(network STATIC ${SRC})

target_include_directories(network
    PUBLIC ${PROJECT_SOURCE_DIR}/include/Network
    PRIVATE ${PROJECT_SOURCE_DIR}/src/Network)

target_link_libraries(network
  sfml-graphics
  sfml-audio
  sfml-network
  sfml-window
  sfml-system
  )


