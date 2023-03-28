cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)
set(ASF_DIR "extern/asf/${KS_BOARD}")

# FreeRTOS
get_filename_component(FREERTOS_CONFIG_FILE_DIRECTORY "${ASF_DIR}/config" ABSOLUTE)
set(FREERTOS_CONFIG_FILE_DIRECTORY "${FREERTOS_CONFIG_FILE_DIRECTORY}" CACHE STRING "")
set(FREERTOS_HEAP "4" CACHE STRING "")
set(FREERTOS_PORT "${KS_FREERTOS_PORT}" CACHE STRING "")
add_subdirectory(extern/freertos)

# ASF
add_subdirectory("extern/asf/${KS_BOARD}")

# Reliance Edge
add_subdirectory(extern/littlefs)

# Nameof header-only library
set(NAMEOF_OPT_BUILD_EXAMPLES OFF)
set(NAMEOF_OPT_BUILD_TESTS OFF)
set(NAMEOF_OPT_INSTALL OFF)
add_subdirectory(extern/nameof)

# Kronos packet library
add_subdirectory(extern/kronos-packet)

# Glob source files
set(KRONOS_SRC
        # KRONOS
        "core/ks_framework.cpp"
        "core/bus/ks_bus.cpp"
        "core/component/ks_component_base.cpp"
        "core/component/ks_component_passive.cpp"
        "core/component/ks_component_queued.cpp"
        "core/component/ks_component_active.cpp"

        "core/utils/error/ks_error.cpp"

        # KRONOS DRIVERS
        "drivers/arch/asf/ks_gpio.cpp"

        "drivers/arch/asf/clock/ks_clock.cpp"
        "drivers/arch/asf/protocols/ks_usart.cpp"
        "drivers/arch/asf/protocols/ks_i2c.cpp"
        "drivers/arch/asf/protocols/ks_spi.cpp"

        "drivers/file_system/apollo_format/ks_apollo_format.cpp"
        "drivers/file_system/ks_file.cpp"
        "drivers/file_system/ks_filesystem.cpp"

        "drivers/sensors/thermistor/ks_thermistor.cpp"

        "drivers/storage/ks_ram.cpp")

# Kronos Core
file(GLOB_RECURSE KRONOS_CORE_SRC "core/**.cpp")
list(APPEND KRONOS_SRC ${KRONOS_CORE_SRC})

# Kronos Modules
file(GLOB_RECURSE KRONOS_MODULES_SRC "modules/**.cpp")
list(APPEND KRONOS_SRC ${KRONOS_MODULES_SRC})

# Create SAM library target
add_mcu_library(Kronos ${KRONOS_SRC})

# Include directories
set(KRONOS_INCLUDE_DIRS
        "config"

        # KRONOS
        "./"
        "core"
        "core/bus"
        "core/component"
        "core/macros"
        "core/module"
        "core/types"
        "core/utils/error"

        # KRONOS DRIVERS
        "drivers"
        "drivers/clock"
        "drivers/file_system"
        "drivers/file_system/apollo_format"
        "drivers/protocols"
        "drivers/sensors/thermistor"
        "drivers/storage"

        # KRONOS MODULES
        "modules/communication_handler"
        "modules/communication_handler/components/command_dispatcher"
        "modules/communication_handler/components/command_listener"
        "modules/communication_handler/components/command_scheduler"
        "modules/communication_handler/components/command_transmitter"

        "modules/file_manager"
        "modules/file_manager/components"

        "modules/housekeeping"
        "modules/housekeeping/components"

        "modules/parameter_database"
        "modules/parameter_database/components"

        "modules/scheduler"
        "modules/scheduler/components"

        "modules/telemetry"
        "modules/telemetry/components"

        "modules/worker"
        "modules/worker/components")

target_include_directories(Kronos PUBLIC ${KRONOS_INCLUDE_DIRS})

# Debug definitions
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DKS_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DKS_DEBUG")

target_sources(Kronos PUBLIC kronos.cpp)

# Extra compile definitions
if(DEFINED KS_COMPILE_DEFINITIONS)
    target_compile_definitions(Kronos PUBLIC ${KS_COMPILE_DEFINITIONS})
endif()

# Precompile types
target_precompile_headers(Kronos PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/core/ks_pch.h>")

# Links libraries
target_link_libraries(Kronos PUBLIC asf littlefs KronosPacket nameof::nameof)
