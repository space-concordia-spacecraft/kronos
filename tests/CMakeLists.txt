cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 14)

project(Kronos_Tests)

set(KRONOS_TESTS_SRC "${KRONOS_EXTRA_SRC}" "src/main.cpp")

add_sam_executable(Kronos_Tests "${KRONOS_TESTS_SRC}")

target_include_directories(Kronos_Tests PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/include/components"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/include/services"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/include/types"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/include/utilities"
)

target_link_libraries(Kronos_Tests Kronos)
