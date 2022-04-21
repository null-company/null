cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Protobuf CONFIG REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

if(MSVC AND protobuf_MSVC_STATIC_RUNTIME)
    foreach(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif(${flag_var} MATCHES "/MD")
    endforeach()
endif()

set(INCROOT ${PROJECT_SOURCE_DIR}/include/Serialization/)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Serialization/)

set(SRC
        ${INCROOT}/serializable.h
        ${SRCROOT}/scene.proto
        )

set(libname serialization)

add_library(${libname} STATIC ${SRC})
target_link_libraries(${libname} protobuf::libprotobuf)
protobuf_generate(TARGET ${libname})
target_include_directories(${libname}
        PUBLIC ${PROJECT_SOURCE_DIR}/include/Serialization
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
        PRIVATE ${PROJECT_SOURCE_DIR}/src)
