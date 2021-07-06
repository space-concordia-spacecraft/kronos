cmake_minimum_required(VERSION 3.14)
project(Kronos)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(Kronos_lib)

set(DEMO_SRC demo.cpp)

add_sam_executable(Kronos_Demo ${DEMO_SRC})

target_include_directories(Kronos_Demo PUBLIC
        "${KRONOS_INCLUDE_DIRS}"
        "${CMAKE_CURRENT_SOURCE_DIR}/Kronos_lib/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/Kronos_lib/include/components"
        "${CMAKE_CURRENT_SOURCE_DIR}/Kronos_lib/include/services"
        "${CMAKE_CURRENT_SOURCE_DIR}/Kronos_lib/include/types"
        "${CMAKE_CURRENT_SOURCE_DIR}/Kronos_lib/include/utilities"
)

target_link_libraries(Kronos_Demo Kronos)
