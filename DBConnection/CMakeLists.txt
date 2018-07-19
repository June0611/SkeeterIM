cmake_minimum_required(VERSION 3.5)
project(DBConnection)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp DBConnectionPool.cpp DBConnectionPool.h)
add_executable(DBConnection ${SOURCE_FILES})
target_link_libraries(DBConnection mysqlcppconn)



