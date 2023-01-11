cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)

project(Kronos_Tests)

set(KRONOS_TESTS_SRC "${KRONOS_EXTRA_SRC}"
        "src/unit/ApolloTests.cpp"
        "src/unit/FileTests.cpp"
        "src/unit/QueueTests.cpp"
        "src/KronosTest.cpp"
        "src/main.cpp"
)

add_sam_executable(Kronos_Tests "${KRONOS_TESTS_SRC}")

target_include_directories(Kronos_Tests PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        # KRONOS
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/core"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/core/bus"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/core/component"

        # KRONOS MODULES
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/core/modules/logger"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/core/modules/file_system"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/core/modules/file_system/file"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/core/modules/file_system/apollo_format"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/core/modules/parameter_database"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/core/modules/telemetry_logger"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/core/modules/scheduler"

        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/macros"

        # KRONOS OS RELATED IMPLEMENTATIONS
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/os"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/os/freertos"

        # KRONOS DRIVERS
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/drivers"

        # KRONOS COMPONENTS
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/components"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/components/health_monitor"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib/components/led_blinker"
)

target_link_libraries(Kronos_Tests Kronos)
