cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 14)

project(Kronos)
#add_subdirectory(extern/HDF5)

message(STATUS "Using the following variables to include Kronos source:")
message("")
message("ARM_CPU: \t\t\t${ARM_CPU}")
message("SAM_ARCH: \t\t\t${SAM_ARCH}")
message("SAM_MCU: \t\t\t${SAM_MCU}")
message("SAM_BOARD: \t\t\t${SAM_BOARD}")
if(USE_UPLOADER)
    message("SAM_UPLOAD_TOOL: \t${SAM_UPLOAD_TOOL}")
endif()
message("")
# Glob source files
set(KRONOS_SRC
        # -------------------------- FREERTOS FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RTOS/freertos/FreeRTOSV10.0.0/Source/croutine.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RTOS/freertos/FreeRTOSV10.0.0/Source/event_groups.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RTOS/freertos/FreeRTOSV10.0.0/Source/list.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RTOS/freertos/FreeRTOSV10.0.0/Source/queue.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RTOS/freertos/FreeRTOSV10.0.0/Source/tasks.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RTOS/freertos/FreeRTOSV10.0.0/Source/timers.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/MemMang/heap_1.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/GCC/ARM_CM7/port.c"

        # -------------------------- HAL FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/src/hal_atomic.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/src/hal_can_async.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/src/hal_dac_sync.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/src/hal_delay.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/src/hal_gpio.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/src/hal_init.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/src/hal_io.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/src/hal_mci_sync.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/src/hal_sleep.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/src/hal_spi_m_sync.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/src/hal_usart_sync.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/hal/utils/src/utils_assert.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/utils/src/utils_event.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/utils/src/utils_list.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/utils/src/utils_ringbuffer.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/utils/src/utils_syscalls.c"

        # -------------------------- HPL FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/core/hpl_core_m7_base.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/core/hpl_init.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/dac/hpl_dac.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/hsmci/hpl_hsmci.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/mcan/hpl_mcan.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/pmc/hpl_pmc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/pmc/hpl_sleep.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/spi/hpl_spi.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/uart/hpl_uart.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/xdmac/hpl_xdmac.c"

        # -------------------------- COMMON FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/common/memory/sd_mmc/sd_mmc.c"

        # -------------------------- RELIANCE EDGE FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/bdev/bdev.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/core/driver/blockio.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/core/driver/buffer.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/core/driver/core.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/core/driver/dir.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/core/driver/format.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/core/driver/imap.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/core/driver/imapextern.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/core/driver/imapinline.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/core/driver/inode.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/core/driver/inodedata.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/core/driver/volume.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/fse/fse.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/os/freertos/services/osassert.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/os/freertos/services/osbdev.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/os/freertos/services/osclock.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/os/freertos/services/osmutex.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/os/freertos/services/osoutput.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/os/freertos/services/ostask.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/os/freertos/services/ostimestamp.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/posix/path.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/posix/posix.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/tests/posix/fsstress.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/tests/util/atoi.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/tests/util/math.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/tests/util/printf.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/tests/util/rand.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/tools/getopt.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/tools/toolcmn.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/util/bitmap.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/util/crc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/util/memory.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/util/namelen.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/util/sign.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/util/string.c"

        # -------------------------- V71 SPECIFIC FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/boards/sam_v71_q21_orthus/drivers/driver_init.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/boards/sam_v71_q21_orthus/memory/sd_mmc_start.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/boards/samv71/gcc/system_samv71q21.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/boards/samv71/gcc/gcc/startup_samv71q21.c"

        # -------------------------- CORE FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/components/ks_component_base.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/components/ks_component_passive.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/components/ks_component_queued.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/components/ks_component_active.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/components/services/ks_telemetry_logger.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/components/services/ks_file_manager.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/components/services/ks_health_monitor.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/components/services/ks_logger.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/components/services/ks_timer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/components/services/ks_rate_group.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/components/services/ks_command_dispatcher.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/components/services/ks_parameter_database.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/components/services/ks_communications.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/components/services/ks_file_transfer.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/utils/ks_profiler.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/utils/ks_apollo_format.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/utils/ks_driver.cpp"


        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/kronos.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/ks_bus.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/src/ks_framework.cpp"
)

add_sam_library(Kronos ${KRONOS_SRC})

set(KRONOS_INCLUDE_DIRS
        # -------------------------- FREERTOS FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RTOS/"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RTOS/freertos/FreeRTOSV10.0.0"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RTOS/freertos/FreeRTOSV10.0.0/Source/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RTOS/freertos/FreeRTOSV10.0.0/Source/portable/GCC/ARM_CM7"

        # -------------------------- CONFIG FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/config"

        # -------------------------- HAL FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/hal/utils/include"

        # -------------------------- HPL FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/core"
        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/mcan"
        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/pio"
        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/pmc"
        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/spi"
        "${CMAKE_CURRENT_SOURCE_DIR}/hpl/uart"

        # -------------------------- HRI FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/hri"

        # -------------------------- COMMON FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/common/memory/sd_mmc"

        # -------------------------- RELIANCE EDGE FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/core/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/os/freertos/include"

        # -------------------------- V71 SPECIFIC FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/boards/sam_v71_q21_orthus/drivers"
        "${CMAKE_CURRENT_SOURCE_DIR}/boards/sam_v71_q21_orthus/memory"
        "${CMAKE_CURRENT_SOURCE_DIR}/boards/sam_v71_q21_orthus"

        "${CMAKE_CURRENT_SOURCE_DIR}/boards/samv71/include"

        # -------------------------- CORE FILES -------------------------- #
        "${CMAKE_CURRENT_SOURCE_DIR}/core/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/include/types"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/include/components"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/include/components/services"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/include/utils"
)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DKS_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DKS_DEBUG")
target_include_directories(Kronos PUBLIC ${KRONOS_INCLUDE_DIRS} )
#target_include_directories(Kronos PUBLIC ${HDF5_INCLUDE_DIRS} )

target_compile_definitions(Kronos PUBLIC CSP_FREERTOS)

#target_link_libraries(Kronos PUBLIC ${HDF5_LIBRARIES})
