cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 14)

project(Kronos_Tests)

add_sam_executable(Kronos_Tests
        src/main.cpp
)

target_link_libraries(Kronos_Tests Kronos)