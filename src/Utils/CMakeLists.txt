cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/GraphicUtils/)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Utils/)

set(SRC
        ${SRCROOT}/Vector.cpp
        ${INCROOT}/Vector.hpp
        )

add_library(graphic-utils STATIC ${SRC})
target_include_directories(graphic-utils
        PUBLIC ${PROJECT_SOURCE_DIR}/include/GraphicUtils
        PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(graphic-utils
        sfml-graphics
        sfml-audio
        sfml-network
        sfml-window
        sfml-system
        )


