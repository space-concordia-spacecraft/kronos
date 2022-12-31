cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 14)

project(Kronos_Tests)

set(KRONOS_TESTS_SRC "${KRONOS_EXTRA_SRC}"
        "src/unit/ApolloTests.cpp"
        "src/unit/FileTests.cpp"
        "src/unit/QueueTests.cpp"
        "src/unit/TelemetryLoggerTests.cpp"
        "src/KronosTest.cpp"
        "src/main.cpp"
)

add_sam_executable(Kronos_Tests "${KRONOS_TESTS_SRC}")

target_include_directories(Kronos_Tests PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "../lib/legacy/include"
        "../lib/legacy/include/components"
        "../lib/legacy/include/services"
        "../lib/legacy/include/types"
        "../lib/legacy/include/utilities"
)

target_link_libraries(Kronos_Tests Kronos)
