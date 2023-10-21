cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(LLVM)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(LLVM game_of_life.c sim.c)

target_link_libraries(LLVM ${SDL2_LIBRARIES})