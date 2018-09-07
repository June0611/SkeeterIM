cmake_minimum_required(VERSION 3.5)
project(CMakeCase)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

message(STATUS "This is BINARY dir " ${CMakeCase_BINARY_DIR})
message(STATUS "This is SOURCE dir "${CMakeCase_SOURCE_DIR})

set(SOURCE_FILES main.cpp)
add_executable(CMakeCase ${SOURCE_FILES})



