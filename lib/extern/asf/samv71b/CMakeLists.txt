set(ASF_SRC
        # FREERTOS
        "thirdparty/RTOS"
        "thirdparty/RTOS/freertos/FreeRTOSV10.0.0/rtos_port.c"

        # ASF START
        "stdio_start.c"
        "spi_nor_flash_main.c"
        "driver_init.c"
        "atmel_start.c"

        # ASF
        "${KS_BOARD}/gcc/system_${KS_MCU}.c"
        "${KS_BOARD}/gcc/gcc/startup_${KS_MCU}.c"

        # ASF STDIO REDIRECT
        "stdio_redirect/stdio_io.c"

        # ASF SPI NOR FLASH
        "spi_nor_flash/spi_nor_flash.c"
        "spi_nor_flash/n25q256a/n25q256a.c")

file(GLOB_RECURSE ASF_DRIVERS_SRC
        "hpl/**.c"
        "hri/**.c"
        "hal/**.c")
list(APPEND ASF_SRC ${ASF_DRIVERS_SRC})

set(ASF_EXTRA_SRC
        "hal/utils/src/utils_assert.c"
        "hal/utils/src/utils_event.c"
        "hal/utils/src/utils_list.c"
        "hal/utils/src/utils_ringbuffer.c"
        "hal/utils/src/utils_syscalls.c"
        "stdio_redirect/gcc/read.c"
        "stdio_redirect/gcc/write.c")

set(ASF_INCLUDE_DIRS
        # FREERTOS
        "thirdparty/RTOS"
        "thirdparty/RTOS/freertos/FreeRTOSV10.0.0"

        # ASF
        "./"

        # ASF CMSIS
        "CMSIS/Core/Include"

        # ASF CONFIG
        "config"

        # ASF HAL
        "hal/include"
        "hal/utils/include"

        #ASF HPL
        "hpl/core"
        "hpl/efc"
        "hpl/mcan"
        "hpl/pio"
        "hpl/pmc"
        "hpl/qspi"
        "hpl/rtc"
        "hpl/spi"
        "hpl/twihs"
        "hpl/uart"
        "hpl/xdmac"

        # ASF HRI
        "hri"

        # ASF SAM
        "${KS_BOARD}/include"
        "${KS_BOARD}/include/component"
        "${KS_BOARD}/include/instance"
        "${KS_BOARD}/include/pio"

        # ASF STDIO REDIRECT
        "stdio_redirect"

        # ASF SD MMC
        "spi_nor_flash"
        "spi_nor_flash/n25q256a")

add_mcu_library("asf" ${ASF_SRC})
target_include_directories("asf" PUBLIC ${ASF_INCLUDE_DIRS})
target_sources("asf" PUBLIC ${ASF_EXTRA_SRC})
target_link_libraries("asf" PUBLIC freertos_kernel)