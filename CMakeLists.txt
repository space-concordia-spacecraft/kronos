cmake_minimum_required(VERSION 3.14)
project(Kronos)

set(CMAKE_CXX_STANDARD 14)


set(SOURCE_FILES main.cpp)
add_executable(Kronos_run ${SOURCE_FILES})

include_directories(Kronos_lib Kronos_lib/include Kronos_lib/include/types)
add_subdirectory(Kronos_lib)

target_link_libraries(Kronos_run Kronos_lib)

add_subdirectory(Google_tests)

