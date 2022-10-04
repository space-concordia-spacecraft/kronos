cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 14)

project(Kronos)

add_subdirectory(lib)
add_subdirectory(tests)

set(DEMO_SRC demo.cpp)

add_sam_executable(Kronos_Demo "${DEMO_SRC}")

target_include_directories(Kronos_Demo PUBLIC
        "lib/core/include"
        "lib/core/include/components"
        "lib/core/include/services"
        "lib/core/include/types"
        "lib/core/include/utilities"
)

target_link_libraries(Kronos_Demo Kronos)