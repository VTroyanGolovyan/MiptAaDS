cmake_minimum_required(VERSION 3.21)
project(C++)

set(CMAKE_CXX_STANDARD 20)

add_executable(LRU LRU.cpp)

add_executable(LRUTest LRUtests.cpp)
