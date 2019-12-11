cmake_minimum_required(VERSION 3.5)
project(armed-bandits)

set(LD_LIBRARY_PATH boost/boost_1_71_0/stage/lib:$LD_LIBRARY_PATH)
set(DYLD_LIBRARY_PATH boost/boost_1_71_0/stage/lib:$DYLD_LIBRARY_PATH)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY executables)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic -std=c++11 -I boost/boost_1_71_0")
SET(GCC_COVERAGE_LINK_FLAGS    "-L boost/boost_1_71_0/stage/lib -lboost_filesystem -lboost_system")

#link_libraries(stdc++fs)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

include_directories(includes)

set(SOURCES
    source/RandomServices.cpp
    source/Bandit.cpp
    source/NormalBandit.cpp
    source/BernoulliBandit.cpp
    source/Problem.cpp
    source/NormalProblem.cpp
    source/BernoulliProblem.cpp
    source/Player.cpp
    source/EpsilonGreedy.cpp
    source/InitialOptimism.cpp
    source/UpperConfidenceBound.cpp
    source/GradientAscent.cpp
    source/Result.cpp
    source/Run.cpp
    source/Experiment.cpp
    source/main.cpp
)

set(INCLUDES
    includes/RandomServices.hpp
    includes/Bandit.hpp
    includes/NormalBandit.hpp
    includes/BernoulliBandit.hpp
    includes/Problem.hpp
    includes/BernoulliProblem.hpp
    includes/Player.hpp
    includes/EpsilonGreedy.hpp
    includes/InitialOptimism.hpp
    include/UpperConfidenceBound.cpp
    includes/GradientAscent.hpp
    includes/Result.hpp
    includes/Run.hpp
    includes/Experiment.hpp
)

add_executable(armed-bandits ${SOURCES})
target_include_directories(armed-bandits PRIVATE includes)
