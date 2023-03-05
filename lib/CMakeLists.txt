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
add_subdirectory(extern/reliance-edge)

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

        # KRONOS DRIVERS
        "drivers/asf/ks_gpio.cpp"
        "drivers/asf/ks_usart.cpp"
        "drivers/asf/ks_i2c.cpp"
        "drivers/asf/ks_spi.cpp"

        "components/led_blinker/ks_led_blinker.cpp")

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
        # KRONOS
        "./"
        "core"
        "core/bus"
        "core/component"
        "core/module"

        # KRONOS MACROS
        "core/macros"

        # KRONOS MODULES
        "modules/clk"
        "modules/clk/components"
        "modules/cmd"
        "modules/cmd/components/command_dispatcher"
        "modules/cmd/components/command_listener"
        "modules/cmd/components/command_scheduler"
        "modules/cmd/components/command_transmitter"
        "modules/cmd/utils"
        "modules/fs"
        "modules/fs/components"
        "modules/fs/utils"
        "modules/fs/utils/apollo_format"
        "modules/fs/utils/file"
        "modules/fs/utils/file_system"
        "modules/hlt"
        "modules/hlt/components"
        "modules/hlt/components/health_monitor"
        "modules/lcn"
        "modules/lcn/components/lcn_master"
        "modules/lcn/components/lcn_slave"
        "modules/log"
        "modules/log/components"
        "modules/log/utils"
        "modules/prf"
        "modules/prf/components"
        "modules/prf/utils"
        "modules/prm"
        "modules/prm/components"
        "modules/sch"
        "modules/sch/components/scheduler"
        "modules/sch/components/worker"
        "modules/sch/config"
        "modules/thm"
        "modules/thm/components"
        "modules/thm/utils"
        "modules/tlm"
        "modules/tlm/components"
        "modules/wrk"
        "modules/wrk/components"

        # KRONOS OS RELATED IMPLEMENTATIONS
        "os"
        "os/freertos"

        # KRONOS DRIVERS
        "drivers"

        # KRONOS COMPONENTS
        "components"
        "components/led_blinker"
        )
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
target_precompile_headers(Kronos PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/core/ks_types.h>")

# Links libraries
target_link_libraries(Kronos PUBLIC asf reliance_edge KronosPacket nameof::nameof)
