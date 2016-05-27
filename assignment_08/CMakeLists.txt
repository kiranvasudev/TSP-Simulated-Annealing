cmake_minimum_required(VERSION 2.8)

project( Assignment08 )

set(CMAKE_BUILD_TYPE Debug)

add_definitions(-std=c++11)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

include_directories (
    include
    include/matplotlib-cpp
)

add_executable( assignment08
		src/agent.cpp
        	src/environment.cpp
        	src/main.cpp)

#to use matplot
target_link_libraries(assignment08 -lpython2.7)
