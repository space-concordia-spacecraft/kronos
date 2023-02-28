cmake_minimum_required(VERSION 3.14)

project(Kronos)

# Compile flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-expansion-to-defined -Wno-unused-parameter -Wno-unused-function -Wno-volatile")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-expansion-to-defined -Wno-unused-parameter -Wno-unused-function")

add_subdirectory(lib)

set(DEMO_SRC demo.cpp)
add_mcu_executable(Kronos_Demo "${DEMO_SRC}")
target_link_libraries(Kronos_Demo Kronos)
