project(HAHA)

cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)

add_executable(Pupa main.cpp a_plus_b.cpp)
add_executable(PupaTest test.cpp a_plus_b.cpp)

