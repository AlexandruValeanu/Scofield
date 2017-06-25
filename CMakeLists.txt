cmake_minimum_required(VERSION 3.8)
project(BurrowsWheeler_transform)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread -O3")

set(SOURCE_FILES main.cpp)
add_executable(BurrowsWheeler_transform ${SOURCE_FILES})