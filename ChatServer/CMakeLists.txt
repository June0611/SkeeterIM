cmake_minimum_required(VERSION 3.5)
project(ChatServer)

message(STATUS "start running cmake...")


file(GLOB SOURCE_FILES "*.h" "*.cpp" "*.cc")

#set(SOURCE_FILES SOURCE_FILES)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

link_libraries("protobuf")

find_package(Boost 1.67.0 COMPONENTS system filesystem REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})

endif()

add_executable(ChatServer ${SOURCE_FILES})

if(Boost_FOUND)
    target_link_libraries(ChatServer ${Boost_LIBRARIES})

endif()







