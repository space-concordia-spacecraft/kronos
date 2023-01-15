cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)

project(Kronos)

add_subdirectory(extern/kronos-packet)

message(STATUS "Using the following variables to include Kronos source:")
message("")
message("ARM_CPU: \t\t\t${ARM_CPU}")
message("SAM_ARCH: \t\t\t${SAM_ARCH}")
message("SAM_MCU: \t\t\t${SAM_MCU}")
message("SAM_BOARD: \t\t\t${SAM_BOARD}")
message("")

# Board-specific Definitions
include("../build/include/${SAM_ARCH}.cmake")

# Glob source files
set(KRONOS_SRC_REL
        # FREERTOS
        "extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/rtos_port.c"
        "extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/croutine.c"
        "extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/event_groups.c"
        "extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/list.c"
        "extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/queue.c"
        "extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/stream_buffer.c"
        "extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/tasks.c"
        "extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/timers.c"
        "extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/rtos_port.c"
        "extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/GCC/ARM_CM7/port.c"
        "extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/MemMang/heap_4.c"

        # ASF START
        "extern/asf/stdio_start.c"
        "extern/asf/driver_init.c"
        "extern/asf/sd_mmc_start.c"
        "extern/asf/atmel_start.c"

        # ASF SAME70
        "extern/asf/same70b/gcc/system_same70q21b.c"
        "extern/asf/same70b/gcc/gcc/startup_same70q21b.c"

        # ASF STDIO REDIRECT
        "extern/asf/stdio_redirect/stdio_io.c"

        # ASF SD/MMC
        "extern/asf/sd_mmc/sd_mmc.c"

        # RELIANCE EDGE
        "extern/reliance-edge/bdev/bdev.c"
        "extern/reliance-edge/config/redconf.c"
        "extern/reliance-edge/core/driver/blockio.c"
        "extern/reliance-edge/core/driver/buffer.c"
        "extern/reliance-edge/core/driver/buffercmn.c"
        "extern/reliance-edge/core/driver/core.c"
        "extern/reliance-edge/core/driver/dir.c"
        "extern/reliance-edge/core/driver/format.c"
        "extern/reliance-edge/core/driver/imap.c"
        "extern/reliance-edge/core/driver/imapextern.c"
        "extern/reliance-edge/core/driver/imapinline.c"
        "extern/reliance-edge/core/driver/inode.c"
        "extern/reliance-edge/core/driver/inodedata.c"
        "extern/reliance-edge/core/driver/volume.c"
        "extern/reliance-edge/fse/fse.c"
        "extern/reliance-edge/os/freertos/services/osassert.c"
        "extern/reliance-edge/os/freertos/services/osbdev.c"
        "extern/reliance-edge/os/freertos/services/osclock.c"
        "extern/reliance-edge/os/freertos/services/osmutex.c"
        "extern/reliance-edge/os/freertos/services/osoutput.c"
        "extern/reliance-edge/os/freertos/services/ostask.c"
        "extern/reliance-edge/os/freertos/services/ostimestamp.c"
        "extern/reliance-edge/posix/path.c"
        "extern/reliance-edge/posix/posix.c"
        "extern/reliance-edge/util/bitmap.c"
        "extern/reliance-edge/util/crc.c"
        "extern/reliance-edge/util/endian.c"
        "extern/reliance-edge/util/heap.c"
        "extern/reliance-edge/util/memory.c"
        "extern/reliance-edge/util/namelen.c"
        "extern/reliance-edge/util/sign.c"
        "extern/reliance-edge/util/string.c"

        # KRONOS
        "core/ks_framework.cpp"
        "core/bus/ks_bus.cpp"
        "core/component/ks_component_base.cpp"
        "core/component/ks_component_passive.cpp"
        "core/component/ks_component_queued.cpp"
        "core/component/ks_component_active.cpp"

        # KRONOS OS DEPENDENT TYPES
        "kronos.cpp"

        # KRONOS DRIVERS
        "drivers/asf/ks_gpio.cpp"
        "drivers/asf/ks_usart.cpp"

        # KRONOS COMPONENTS
        "modules/health/components/health_monitor/ks_health_monitor.cpp"
        "components/led_blinker/ks_led_blinker.cpp"
        )
foreach (src ${KRONOS_SRC_REL})
    get_filename_component(src_abs "${src}" ABSOLUTE)
    list(APPEND KRONOS_SRC "${src_abs}")
endforeach ()

# ASF Driver Files
file(GLOB_RECURSE KRONOS_DRIVERS_SRC
        "extern/asf/hpl/**.c"
        "extern/asf/hri/**.c"
        "extern/asf/hal/**.c")
list(APPEND KRONOS_SRC ${KRONOS_DRIVERS_SRC})

# Kronos Core
file(GLOB_RECURSE KRONOS_CORE_SRC "core/**.cpp")
list(APPEND KRONOS_SRC ${KRONOS_CORE_SRC})

# Kronos Modules
file(GLOB_RECURSE KRONOS_MODULES_SRC "modules/**.cpp")
list(APPEND KRONOS_SRC ${KRONOS_MODULES_SRC})

# Create SAM library target
add_sam_library(Kronos ${KRONOS_SRC} ${KRONOS_DRIVERS_SRC})

# Include directories
set(KRONOS_INCLUDE_DIRS
        # FREERTOS
        "extern/asf/thirdparty/RTOS"
        "extern/asf/thirdparty/RTOS/freertos"
        "extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0"
        "extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/include"
        "extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/GCC/ARM_CM7"

        # ASF
        "extern/asf"

        # ASF CMSIS
        "extern/asf/CMSIS/Core/Include"

        # ASF CONFIG
        "extern/asf/config"

        # ASF HAL
        "extern/asf/hal/include"
        "extern/asf/hal/utils/include"

        #ASF HPL
        "extern/asf/hpl/core"
        "extern/asf/hpl/mcan"
        "extern/asf/hpl/pio"
        "extern/asf/hpl/pmc"
        "extern/asf/hpl/usart"

        # ASF HRI
        "extern/asf/hri"

        # ASF SAME70
        "extern/asf/same70b/include"
        "extern/asf/same70b/include/component"
        "extern/asf/same70b/include/instance"
        "extern/asf/same70b/include/pio"
        "extern/asf/same70b/include/pio"

        # ASF STDIO REDIRECT
        "extern/asf/stdio_redirect"

        # ASF SD/MMC
        "extern/asf/sd_mmc"

        # RELIANCE EDGE
        "extern/reliance-edge/include"
        "extern/reliance-edge/config"
        "extern/reliance-edge/core/driver"
        "extern/reliance-edge/core/include"
        "extern/reliance-edge/os/freertos/include"
        "extern/reliance-edge/os/freertos/services"

        # KRONOS
        "${CMAKE_CURRENT_SOURCE_DIR}"
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
        "modules/cmd/utils"
        "modules/fs"
        "modules/fs/components"
        "modules/fs/utils"
        "modules/fs/utils/file"
        "modules/fs/utils/apollo_format"
        "modules/health"
        "modules/health/components"
        "modules/health/components/health_monitor"
        "modules/log"
        "modules/log/components"
        "modules/log/utils"
        "modules/params"
        "modules/params/components"
        "modules/sched"
        "modules/sched/components/scheduler"
        "modules/sched/components/worker"
        "modules/tlm"
        "modules/tlm/components"

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

# Extra sources
set(KRONOS_EXTRA_SRC
        # This is done for functions that are not used in the library but that are needed later.
        # Because they're not used they're initially yeeted.
        "extern/asf/hal/utils/src/utils_syscalls.c"
        "extern/asf/stdio_redirect/gcc/read.c"
        "extern/asf/stdio_redirect/gcc/write.c")

target_sources(Kronos PUBLIC ${KRONOS_EXTRA_SRC})

# Debug definitions
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DKS_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DKS_DEBUG")

# Extra compile definitions
if (DEFINED KS_COMPILE_DEFINITIONS)
    target_compile_definitions(Kronos PUBLIC ${KS_COMPILE_DEFINITIONS})
endif ()

# Precompile types
target_precompile_headers(Kronos PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/core/ks_types.h>")

# Links libraries
target_link_libraries(Kronos PUBLIC KronosPacket)
