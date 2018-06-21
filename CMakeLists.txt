cmake_minimum_required(VERSION 3.0)
project(avl_tree)

include_directories(${avl_tree_SOURCE_DIR})

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17 -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
endif()

add_library(counted counted.h counted.cpp fault_injection.h fault_injection.cpp mman.h mman.cpp)
add_library(gtest gtest/gtest-all.cc gtest/gtest_main.cc)
add_executable(avl_tree_testing avl_tree.h avl_tree.tpp test.cpp)
target_link_libraries(avl_tree_testing counted gtest)