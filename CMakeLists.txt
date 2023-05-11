cmake_minimum_required(VERSION 3.10)
project(catan-simulation)
include(CTest)

find_package(Catch2 REQUIRED)

enable_testing()

add_subdirectory(src)

file(COPY config_files DESTINATION ${CMAKE_BINARY_DIR})

include_directories(boardcell PRIVATE src)
include_directories(tile PRIVATE src)
include_directories(road PRIVATE src)
include_directories(board PRIVATE src)

target_link_directories(boardcell PRIVATE src/)
target_link_directories(tile PRIVATE src/)
target_link_directories(road PRIVATE src/)
target_link_directories(board PRIVATE src/)

add_subdirectory(tests)

add_executable(simulation simulation.cpp)
target_link_libraries(simulation game default_player)