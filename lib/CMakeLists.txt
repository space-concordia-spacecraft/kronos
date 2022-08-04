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
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/event_groups.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/list.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/queue.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/tasks.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/timers.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/memcpy.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/portable/MemMang/heap_4.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/portable/GCC/ARM_CM7/r0p1/port.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_bridge.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_buffer.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_conn.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_crc32.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_debug.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_endian.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_hex_dump.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_iflist.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_init.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_io.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_port.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_promisc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_qfifo.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_route.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_service_handler.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_services.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/csp_sfp.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/arch/freertos/csp_clock.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/arch/freertos/csp_malloc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/arch/freertos/csp_queue.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/arch/freertos/csp_semaphore.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/arch/freertos/csp_system.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/arch/freertos/csp_thread.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/arch/freertos/csp_time.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/arch/csp_time.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/arch/csp_system.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/drivers/usart/usart_sam.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/drivers/usart/usart_kiss.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/interfaces/csp_if_can.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/interfaces/csp_if_can_pbuf.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/interfaces/csp_if_i2c.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/interfaces/csp_if_kiss.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/interfaces/csp_if_lo.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/interfaces/csp_if_zmqhub.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/rtable/csp_rtable.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/rtable/csp_rtable_cidr.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/rtable/csp_rtable_static.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/transport/csp_rdp.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/transport/csp_udp.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/utils/interrupt/interrupt_sam_nvic.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/services/serial/usart_serial.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/services/storage/ctrl_access/ctrl_access.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/utils/stdio/read.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/utils/stdio/write.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/matrix/matrix.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/mcan/mcan.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/mpu/mpu.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/pio/pio.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/pio/pio_handler.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/pmc/pmc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/pmc/sleep.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/tc/tc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/uart/uart.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/usart/usart.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/hsmci/hsmci.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/xdmac/xdmac.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/components/memory/sd_mmc/sd_mmc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/components/memory/sd_mmc/sd_mmc_mem.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/components/memory/sd_mmc/sd_mmc_spi.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/services/clock/${SAM_ARCH}/sysclk.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/boards/${SAM_BOARD}/init.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils/cmsis/${SAM_ARCH}/source/templates/system_${SAM_ARCH}.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils/cmsis/${SAM_ARCH}/source/templates/gcc/startup_${SAM_ARCH}.c"

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
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/config/redconf.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/ks_component_base.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/ks_component_passive.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/ks_component_queued.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/ks_component_active.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/services/ks_telemetry_logger.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/services/ks_file_manager.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/services/ks_health_monitor.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/services/ks_logger.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/services/ks_timer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/services/ks_rate_group.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/services/ks_command_dispatcher.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/services/ks_parameter_database.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/drivers/ks_can.cpp"
        #        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/drivers/ks_uart.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/services/ks_communications.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/services/ks_file_transfer.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/ks_profiler.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/ks_apollo_format.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/src/kronos.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/src/ks_bus.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ks_framework.cpp"
)

add_sam_library(Kronos ${KRONOS_SRC})

set(KRONOS_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/types"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/components"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/components/services"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/components/drivers"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/utils"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/portable/GCC/ARM_CM7/r0p1"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/config"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/boards/${SAM_BOARD}"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils/cmsis/${SAM_ARCH}/source/templates"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils/cmsis/${SAM_ARCH}/include"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/utils"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils/fpu"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/services/serial/sam_uart"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/matrix"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/pmc"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/services/gpio"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/boards"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/pio"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/boards"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils/header_files"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/tc"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/services/ioport"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/mpu"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/hsmci"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/thirdparty/CMSIS/Include"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/thirdparty/CMSIS/Lib/GCC"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/services/clock"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/services/delay"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/utils/stdio/stdio_serial"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils/preprocessor"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/services/serial"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/services/storage/ctrl_access"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/usart"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/uart"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/mcan"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/xdmac"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/components/memory/sd_mmc"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/include/"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/libcsp/src/"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/core/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/os/freertos/include"
        #"${CMAKE_CURRENT_SOURCE_DIR}/extern/HDF5"
)

set(KRONOS_EXTRA_SRC "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils/syscalls/gcc/syscalls.c" CACHE STRING "Extra source code for the Kronos library. To be included when compiling an executable." FORCE)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DKS_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DKS_DEBUG")
target_include_directories(Kronos PUBLIC ${KRONOS_INCLUDE_DIRS} )
#target_include_directories(Kronos PUBLIC ${HDF5_INCLUDE_DIRS} )

target_compile_definitions(Kronos PUBLIC CSP_FREERTOS)

#target_link_libraries(Kronos PUBLIC ${HDF5_LIBRARIES})
