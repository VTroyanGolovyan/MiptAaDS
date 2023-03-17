project(Pupa)

cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)


set(CMAKE_CXX_COMPILE_FLAGS -pthread)

add_subdirectory(mylib)

include_directories(mylib)

add_executable(PupaRun 
  main.cpp
  calc.cpp
)

target_link_libraries(PupaRun a_xor_b)

add_executable(PupaRunTests 
  test.cpp
  calc.cpp
)

install(
  TARGETS PupaRun a_xor_b
  RUNTIME DESTINATION bin 
  LIBRARY DESTINATION lib
)
