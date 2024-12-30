cmake_minimum_required(VERSION 3.10)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin")
set(CMAKE_CXX_STANDARD 20)

project(
    test_yadro
    LANGUAGES CXX)

add_subdirectory(src)

option(BUILD_TESTS "Build the tests" ON)
if(BUILD_TESTS)
    add_subdirectory("extern/googletest")
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
    mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
    )
    set_target_properties(gtest PROPERTIES FOLDER extern)
    set_target_properties(gtest_main PROPERTIES FOLDER extern)
    set_target_properties(gmock PROPERTIES FOLDER extern)
    set_target_properties(gmock_main PROPERTIES FOLDER extern)
endif()

