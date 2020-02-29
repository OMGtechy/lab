cmake_minimum_required(VERSION 3.1.0)
project(lab VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lab source/pool.cpp)
target_include_directories(lab PRIVATE include)

include(CTest)
enable_testing()
add_subdirectory(test)