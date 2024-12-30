add_executable(main main.cpp check_format.cpp table.cpp calc_profit.cpp)
target_include_directories(main PUBLIC "../inc/")

add_library(format_lib STATIC check_format.cpp)
target_include_directories(format_lib PRIVATE "../inc/")

add_library(calc_profit_lib STATIC check_format.cpp calc_profit.cpp table.cpp)
target_include_directories(calc_profit_lib PRIVATE "../inc/")