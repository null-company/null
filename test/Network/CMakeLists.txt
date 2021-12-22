enable_testing()
include(GoogleTest)
add_executable(
        main_test
        main.cpp ClientNetworkManagerTest.cpp)
target_link_libraries(
        main_test
        network
        gtest_main
)
target_include_directories(main_test
        PUBLIC ${PROJECT_SOURCE_DIR}/include/Network
        PRIVATE ${PROJECT_SOURCE_DIR}/src/Network)
set("TEST_CATCH_EXCEPTIONS=0")
add_test(NAME  main_test
        COMMAND main_test)

gtest_discover_tests(main_test)