# cmake_minimum_required(VERSION <specify CMake version here>)
project(projekt)

set(CMAKE_CXX_STANDARD 11)

add_executable(projekt main.cpp node.cpp bits.cpp hoff.cpp ahoff.cpp model.cpp)