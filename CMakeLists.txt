cmake_minimum_required(VERSION 3.8)
project(Scofield)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread -O3")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")


set(SOURCE_FILES main.cpp strings_utility.cpp strings_utility.h constants.h header.cpp header.h encrypt_decrypt.h encrypt_decrypt.cpp)
add_executable(Scofield ${SOURCE_FILES} strings_utility.cpp constants.h header.cpp header.h encrypt_decrypt.h encrypt_decrypt.cpp)