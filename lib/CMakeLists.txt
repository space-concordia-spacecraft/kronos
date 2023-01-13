include(../build/include/same70.cmake)

cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)

project(Kronos)

message(STATUS "Using the following variables to include Kronos source:")
message("")
message("ARM_CPU: \t\t\t${ARM_CPU}")
message("SAM_ARCH: \t\t\t${SAM_ARCH}")
message("SAM_MCU: \t\t\t${SAM_MCU}")
message("SAM_BOARD: \t\t\t${SAM_BOARD}")
if (USE_UPLOADER)
    message("SAM_UPLOAD_TOOL: \t${SAM_UPLOAD_TOOL}")
endif ()
message("")

# Glob source files
set(KRONOS_SRC
        # FREERTOS
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/rtos_port.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/croutine.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/event_groups.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/list.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/queue.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/stream_buffer.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/tasks.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/timers.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/rtos_port.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/GCC/ARM_CM7/port.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/MemMang/heap_4.c"

        # ASF
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/stdio_start.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/driver_init.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/sd_mmc_start.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/atmel_start.c"

        # ASF SAME70
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/same70b/gcc/system_same70q21b.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/same70b/gcc/gcc/startup_same70q21b.c"

        # ASF STDIO REDIRECT
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/stdio_redirect/stdio_io.c"

        # ASF SD/MMC
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/sd_mmc/sd_mmc.c"

        # RELIANCE EDGE
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/bdev/bdev.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/config/redconf.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/core/driver/blockio.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/core/driver/buffer.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/core/driver/buffercmn.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/core/driver/core.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/core/driver/dir.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/core/driver/format.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/core/driver/imap.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/core/driver/imapextern.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/core/driver/imapinline.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/core/driver/inode.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/core/driver/inodedata.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/core/driver/volume.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/fse/fse.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/os/freertos/services/osassert.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/os/freertos/services/osbdev.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/os/freertos/services/osclock.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/os/freertos/services/osmutex.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/os/freertos/services/osoutput.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/os/freertos/services/ostask.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/os/freertos/services/ostimestamp.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/posix/path.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/posix/posix.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/util/bitmap.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/util/crc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/util/endian.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/util/heap.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/util/memory.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/util/namelen.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/util/sign.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/util/string.c"

        # KRONOS
        "${CMAKE_CURRENT_SOURCE_DIR}/core/ks_framework.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/bus/ks_bus.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/component/ks_component_base.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/component/ks_component_passive.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/component/ks_component_queued.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/component/ks_component_active.cpp"

        # KRONOS MODULES
        "${CMAKE_CURRENT_SOURCE_DIR}/core/modules/logger/ks_logger.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/modules/file_system/ks_file_system.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/modules/file_system/file/ks_file.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/modules/file_system/apollo_format/ks_apollo_format.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/modules/parameter_database/ks_parameter_database.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/modules/telemetry_logger/ks_telemetry_logger.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/modules/scheduler/ks_scheduler.cpp"

        # KRONOS OS DEPENDENT TYPES
        "${CMAKE_CURRENT_SOURCE_DIR}/kronos.cpp"

        # KRONOS DRIVERS
        "${CMAKE_CURRENT_SOURCE_DIR}/drivers/asf/ks_gpio.cpp"

        # KRONOS COMPONENTS
        "${CMAKE_CURRENT_SOURCE_DIR}/components/health_monitor/ks_health_monitor.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/components/led_blinker/ks_led_blinker.cpp"
        )

file(GLOB_RECURSE KRONOS_SRC_DRIVERS
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/hpl/**.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/hri/**.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/hal/**.c")

add_sam_library(Kronos ${KRONOS_SRC} ${KRONOS_SRC_DRIVERS})

set(KRONOS_INCLUDE_DIRS
        # FREERTOS
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS/freertos"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/GCC/ARM_CM7"

        # ASF
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf"

        # ASF CMSIS
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/CMSIS/Core/Include"

        # ASF CONFIG
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/config"

        # ASF HAL
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/hal/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/hal/utils/include"

        #ASF HPL
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/hpl/core"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/hpl/pio"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/hpl/pmc"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/hpl/uart"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/hpl/usart"

        # ASF HRI
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/hri"

        # ASF SAME70
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/same70b/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/same70b/include/component"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/same70b/include/instance"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/same70b/include/pio"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/same70b/include/pio"

        # ASF STDIO REDIRECT
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/stdio_redirect"

        # ASF SD/MMC
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/sd_mmc"

        # RELIANCE EDGE
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/config"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/core/driver"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/core/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/os/freertos/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/reliance-edge/os/freertos/services"

        # KRONOS
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/core"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/bus"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/component"

        # KRONOS MODULES
        "${CMAKE_CURRENT_SOURCE_DIR}/core/modules/logger"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/modules/file_system"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/modules/file_system/file"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/modules/file_system/apollo_format"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/modules/parameter_database"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/modules/telemetry_logger"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/modules/scheduler"

        "${CMAKE_CURRENT_SOURCE_DIR}/macros"

        # KRONOS OS RELATED IMPLEMENTATIONS
        "${CMAKE_CURRENT_SOURCE_DIR}/os"
        "${CMAKE_CURRENT_SOURCE_DIR}/os/freertos"

        # KRONOS DRIVERS
        "${CMAKE_CURRENT_SOURCE_DIR}/drivers"

        # KRONOS COMPONENTS
        "${CMAKE_CURRENT_SOURCE_DIR}/components"
        "${CMAKE_CURRENT_SOURCE_DIR}/components/health_monitor"
        "${CMAKE_CURRENT_SOURCE_DIR}/components/led_blinker"
        )

set(KRONOS_EXTRA_SRC
        # This is done for functions that are not used in the library but that are needed later.
        # Because they're not used they're initially yeeted.
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/hal/utils/src/utils_syscalls.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/stdio_redirect/gcc/read.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/asf/stdio_redirect/gcc/write.c")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-volatile -Wno-unused-function")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DKS_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DKS_DEBUG")

if (DEFINED KS_COMPILE_DEFINITIONS)
    target_compile_definitions(Kronos PUBLIC ${KS_COMPILE_DEFINITIONS})
endif ()

target_include_directories(Kronos PUBLIC ${KRONOS_INCLUDE_DIRS})
target_sources(Kronos PUBLIC ${KRONOS_EXTRA_SRC})
