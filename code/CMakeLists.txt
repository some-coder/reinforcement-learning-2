cmake_minimum_required(VERSION 3.5)
project(maze)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY executables)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic -std=c++11")
SET(GCC_COVERAGE_LINK_FLAGS    "")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

include_directories(includes)

set(SOURCES
        sources/RandomServices.cpp
        sources/State.cpp
        sources/Maze.cpp
        sources/Player.cpp
        sources/LearningPlayer.cpp
        sources/DynamicProgrammingPlayer.cpp
        sources/MonteCarloPlayer.cpp
        sources/Main.cpp)

set(INCLUDES
        includes/RandomServices.hpp
        includes/State.hpp
        includes/Maze.hpp
        includes/Player.hpp
        includes/LearningPlayer.hpp
        includes/DynamicProgrammingPlayer.hpp
        includes/MonteCarloPlayer.hpp
        includes/Maze.hpp
)

add_executable(maze ${SOURCES})
target_include_directories(maze PRIVATE includes)
