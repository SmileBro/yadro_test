cmake_minimum_required(VERSION 3.10)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")
set(CMAKE_CXX_STANDARD 20)

project(
    test_yadro
    LANGUAGES CXX)

add_subdirectory(src)