cmake_minimum_required(VERSION 3.1.0)
project(lab VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -Werror)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=undefined -fsanitize=address")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=undefined -fsanitize=address")

add_library(lab source/pool.cpp source/region.cpp)
target_include_directories(lab PRIVATE include)

include(CTest)
enable_testing()
add_subdirectory(test)
