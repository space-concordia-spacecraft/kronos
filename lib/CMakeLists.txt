cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)
set(ASF_DIR "extern/asf/${KS_BOARD}")

# FreeRTOS
get_filename_component(FREERTOS_CONFIG_FILE_DIRECTORY "${ASF_DIR}/config" ABSOLUTE)
set(FREERTOS_CONFIG_FILE_DIRECTORY "${FREERTOS_CONFIG_FILE_DIRECTORY}" CACHE STRING "")
set(FREERTOS_HEAP "4" CACHE STRING "")
set(FREERTOS_PORT "${KS_FREERTOS_PORT}" CACHE STRING "")
add_subdirectory(extern/freertos)

# Nameof header-only library
set(NAMEOF_OPT_BUILD_EXAMPLES OFF)
set(NAMEOF_OPT_BUILD_TESTS OFF)
set(NAMEOF_OPT_INSTALL OFF)
add_subdirectory(extern/nameof)

# Kronos packet library
add_subdirectory(extern/kronos-packet)

# Glob source files
set(KRONOS_SRC_REL
        # FREERTOS
        "${ASF_DIR}/thirdparty/RTOS/freertos/FreeRTOSV10.0.0/rtos_port.c"

        # ASF START
        "${ASF_DIR}/stdio_start.c"
        "${ASF_DIR}/spi_nor_flash_main.c"
        "${ASF_DIR}/driver_init.c"
        "${ASF_DIR}/atmel_start.c"

        # ASF
        "${ASF_DIR}/${KS_BOARD}/gcc/system_samv71q21b.c"
        "${ASF_DIR}/${KS_BOARD}/gcc/gcc/startup_samv71q21b.c"

        # ASF STDIO REDIRECT
        "${ASF_DIR}/stdio_redirect/stdio_io.c"

        # ASF SPI NOR FLASH
        "${ASF_DIR}/spi_nor_flash/spi_nor_flash.c"
        "${ASF_DIR}/spi_nor_flash/n25q256a/n25q256a.c"

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
        "components/led_blinker/ks_led_blinker.cpp"
        )
foreach(src ${KRONOS_SRC_REL})
    get_filename_component(src_abs "${src}" ABSOLUTE)
    list(APPEND KRONOS_SRC "${src_abs}")
endforeach()

# ASF Driver Files
file(GLOB_RECURSE KRONOS_DRIVERS_SRC
        "${ASF_DIR}/hpl/**.c"
        "${ASF_DIR}/hri/**.c"
        "${ASF_DIR}/hal/**.c")
list(APPEND KRONOS_SRC ${KRONOS_DRIVERS_SRC})

# Kronos Core
file(GLOB_RECURSE KRONOS_CORE_SRC "core/**.cpp")
list(APPEND KRONOS_SRC ${KRONOS_CORE_SRC})

# Kronos Modules
file(GLOB_RECURSE KRONOS_MODULES_SRC "modules/**.cpp")
list(APPEND KRONOS_SRC ${KRONOS_MODULES_SRC})

# Create SAM library target
add_mcu_library(Kronos ${KRONOS_SRC} ${KRONOS_DRIVERS_SRC})

# Include directories
set(KRONOS_INCLUDE_DIRS
        # FREERTOS
        "${ASF_DIR}/thirdparty/RTOS"
        "${ASF_DIR}/thirdparty/RTOS/freertos/FreeRTOSV10.0.0"
        # ASF
        "${ASF_DIR}"

        # ASF CMSIS
        "${ASF_DIR}/CMSIS/Core/Include"

        # ASF CONFIG
        "${ASF_DIR}/config"

        # ASF HAL
        "${ASF_DIR}/hal/include"
        "${ASF_DIR}/hal/utils/include"

        #ASF HPL
        "${ASF_DIR}/hpl/core"
        "${ASF_DIR}/hpl/mcan"
        "${ASF_DIR}/hpl/pio"
        "${ASF_DIR}/hpl/pmc"
        "${ASF_DIR}/hpl/uart"
        "${ASF_DIR}/hpl/mcan"
        "${ASF_DIR}/hpl/spi"

        # ASF HRI
        "${ASF_DIR}/hri"

        # ASF SAM
        "${ASF_DIR}/${KS_BOARD}/include"
        "${ASF_DIR}/${KS_BOARD}/include/component"
        "${ASF_DIR}/${KS_BOARD}/include/instance"
        "${ASF_DIR}/${KS_BOARD}/include/pio"

        # ASF STDIO REDIRECT
        "${ASF_DIR}/stdio_redirect"

        # ASF SPI NOR FLASH
        "${ASF_DIR}/spi_nor_flash"
        "${ASF_DIR}/spi_nor_flash/n25q256a"

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

# Extra sources
set(KRONOS_EXTRA_SRC
        # This is done for functions that are not used in the library but that are needed later.
        # Because they're not used they're initially yeeted.
        "${ASF_DIR}/hal/utils/src/utils_assert.c"
        "${ASF_DIR}/hal/utils/src/utils_event.c"
        "${ASF_DIR}/hal/utils/src/utils_list.c"
        "${ASF_DIR}/hal/utils/src/utils_ringbuffer.c"
        "${ASF_DIR}/hal/utils/src/utils_syscalls.c"
        "${ASF_DIR}/stdio_redirect/gcc/read.c"
        "${ASF_DIR}/stdio_redirect/gcc/write.c"
        "kronos.cpp")

target_sources(Kronos PUBLIC ${KRONOS_EXTRA_SRC})

# Debug definitions
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DKS_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DKS_DEBUG")

# Extra compile definitions
if(DEFINED KS_COMPILE_DEFINITIONS)
    target_compile_definitions(Kronos PUBLIC ${KS_COMPILE_DEFINITIONS})
endif()

# Precompile types
target_precompile_headers(Kronos PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/core/ks_types.h>")

# Links libraries
target_link_libraries(Kronos PUBLIC freertos_kernel KronosPacket nameof::nameof)
