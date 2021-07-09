cmake_minimum_required(VERSION 3.14)
project(Kronos)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib)
add_subdirectory(tests)

set(DEMO_SRC demo.cpp)

add_sam_executable(Kronos_Demo "${DEMO_SRC}")

target_include_directories(Kronos_Demo PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/include/components"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/include/services"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/include/types"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/include/utilities"
)

target_link_libraries(Kronos_Demo Kronos)