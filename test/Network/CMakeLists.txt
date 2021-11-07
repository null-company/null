enable_testing()
include(GoogleTest)
add_executable(
        main_test
        main.cpp
        DeltaObjects/DeltaObjectsTest.cpp)
target_link_libraries(
        main_test
        gtest_main
)

add_test(NAME  main_test
        COMMAND main_test)

gtest_discover_tests(main_test)