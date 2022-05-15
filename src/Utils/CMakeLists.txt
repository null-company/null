cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/Utils/)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Utils/)

set(SRC
        ${SRCROOT}/Graphic/Vector.cpp
        ${INCROOT}/Graphic/Vector.hpp
        Schedulers/LinearScheduler.cpp ../../include/Utils/Schedulers/LinearScheduler.hpp Schedulers/Timer.cpp ../../include/Utils/Schedulers/Timer.hpp)

add_library(utils STATIC ${SRC})
target_include_directories(utils
        PUBLIC ${PROJECT_SOURCE_DIR}/include/Utils
        PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(utils
        sfml-graphics
        sfml-audio
        sfml-network
        sfml-window
        sfml-system
        )


