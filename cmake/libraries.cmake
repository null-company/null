include(FetchContent)

function(make_dependency_available dependency_name)
    message(STATUS "Downloading ${dependency_name}")
    FetchContent_Declare(${ARGV})
    FetchContent_MakeAvailable(${dependency_name})
    message(STATUS "Finished downloading ${dependency_name}")
endfunction()

#set(YAML_CPP_BUILD_TESTS OFF)
make_dependency_available(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG yaml-cpp-0.6.3
)

#make_dependency_available(
#                googletest
#                URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
#)
if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif (WIN32)
set(GTest_DIR ${googletest_BINARY_DIR})

make_dependency_available(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG d950c9387a94087f478434fb42b95c5a9c746fb2 # todo this needs revision. 2.5.1 doesn't fit for clang
)

make_dependency_available(
        plog
        GIT_REPOSITORY https://github.com/SergiusTheBest/plog
        GIT_TAG 914e799d2b08d790f5d04d1c46928586b3a41250
)

make_dependency_available(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v2.4.1
)

make_dependency_available(
        protobuf
        GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
        GIT_TAG        e842f3fe3ccb96f35478a218808d360300cf3552
)

# https://stackoverflow.com/questions/74844369/how-to-i-use-the-cmake-command-protobuf-generate-when-installing-protobuf-throug
# Get source directory of the Protobuf
FetchContent_GetProperties(protobuf SOURCE_DIR Protobuf_SOURCE_DIR)
message(STATUS "Protobuf source dir: ${Protobuf_SOURCE_DIR}")
# Include the script which defines 'protobuf_generate'
include(${Protobuf_SOURCE_DIR}/cmake/protobuf-generate.cmake)
#protobuf_generate(TARGET proto-stuff IMPORT_DIRS ${Protobuf_SOURCE_DIR}/src)
