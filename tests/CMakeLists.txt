set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin")

add_executable(
  format_test
  format_test.cpp
  file_fixture.hpp
)

target_link_libraries(
  format_test
  GTest::gtest_main
  format_lib
)

target_include_directories(format_test PRIVATE "../inc/")
gtest_discover_tests(format_test)

add_executable(
  calc_profit_test
  calc_profit_test.cpp
  file_fixture.hpp
)

target_link_libraries(
  calc_profit_test
  GTest::gtest_main
  format_lib
  calc_profit_lib
)

target_include_directories(calc_profit_test PRIVATE "../inc/")
gtest_discover_tests(calc_profit_test)