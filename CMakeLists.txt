cmake_minimum_required(VERSION 3.0)
project(avl_tree)

include_directories(${avl_tree_SOURCE_DIR})

add_executable(avl_tree_testing avl_tree.h avl_tree.tpp avl_tree_testing_additional.cpp
        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/gtest_main.cc avl_tree_testing.cpp)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17 -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
endif()

target_link_libraries(avl_tree_testing -lpthread)