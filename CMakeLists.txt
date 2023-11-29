cmake_minimum_required(VERSION 3.20)

set(current_project "Markov")
project(${current_project})

include_directories("include")
add_subdirectory(samples)

file(GLOB_RECURSE ALL_SOURCES_FILES src/*.cpp include/*.h include/*.hpp)
add_library(markovlibs STATIC ${ALL_SOURCES_FILES})