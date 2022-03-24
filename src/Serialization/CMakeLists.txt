cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/Serialization/)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Serialization/)

set(SRC
        ${INCROOT}/serializable.h
        ${INCROOT}/scene.pb.h
        ${SRCROOT}/scene.pb.cc
        ${INCROOT}/scripts.pb.h
        ${SRCROOT}/scripts.pb.cc
        ${INCROOT}/box2d.pb.h
        ${SRCROOT}/box2d.pb.cc
        )

add_library(serialization STATIC ${SRC})
target_include_directories(serialization
        PUBLIC ${PROJECT_SOURCE_DIR}/include/Serialization
        PRIVATE ${PROJECT_SOURCE_DIR}/src)




