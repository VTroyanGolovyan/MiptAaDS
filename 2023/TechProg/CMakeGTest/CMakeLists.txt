cmake_minimum_required(VERSION 3.17)

project(Testing)

set(CMAKE_CXX_STANDARD 20)
find_package(GTest REQUIRED)
add_executable(Test test.cpp)
target_link_libraries(Test GTest::gtest)

