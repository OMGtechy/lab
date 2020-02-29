cmake_minimum_required(VERSION 3.1.0)
project(test_lab)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/Catch2)
include(external/Catch2/contrib/Catch.cmake)

add_executable(test_lab source/test.cpp source/test_pool.cpp)
target_include_directories(test_lab PRIVATE include ../include)
target_link_libraries(test_lab lab Catch2::Catch2)

catch_discover_tests(test_lab)
