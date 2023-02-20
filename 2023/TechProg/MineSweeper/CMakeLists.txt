cmake_minimum_required(VERSION 3.21)

project(Mines)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(Mines main.cpp)

target_link_libraries(Mines sfml-graphics sfml-audio)