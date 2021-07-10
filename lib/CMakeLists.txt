cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 14)

project(Kronos)

message(STATUS "Using the following variables to include Kronos source:")
message("")
message("ARM_CPU: \t\t\t${ARM_CPU}")
message("SAM_ARCH: \t\t\t${SAM_ARCH}")
message("SAM_MCU: \t\t\t${SAM_MCU}")
message("SAM_BOARD: \t\t\t${SAM_BOARD}")
message("USE_UPLOADER: \t\t${USE_UPLOADER}")
message("SAM_UPLOAD_TOOL: \t${SAM_UPLOAD_TOOL}")
message("")

# Glob source files
set(KRONOS_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/event_groups.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/list.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/queue.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/tasks.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/timers.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/portable/MemMang/heap_4.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/portable/GCC/ARM_CM7/r0p1/port.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/utils/interrupt/interrupt_sam_nvic.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/services/serial/usart_serial.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/utils/stdio/read.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/utils/stdio/write.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/matrix/matrix.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/mpu/mpu.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/pio/pio.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/pio/pio_handler.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/pmc/pmc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/pmc/sleep.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/tc/tc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/uart/uart.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/usart/usart.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/services/clock/${SAM_ARCH}/sysclk.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/boards/${SAM_BOARD}/init.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils/cmsis/${SAM_ARCH}/source/templates/system_${SAM_ARCH}.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils/cmsis/${SAM_ARCH}/source/templates/gcc/startup_${SAM_ARCH}.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/bdev/bdev.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/core/driver/blockio.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/core/driver/buffer.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/core/driver/core.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/core/driver/dir.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/core/driver/format.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/core/driver/imap.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/core/driver/imapextern.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/core/driver/imapinline.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/core/driver/inode.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/core/driver/inodedata.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/core/driver/volume.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/fse/fse.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/os/freertos/services/osassert.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/os/freertos/services/osbdev.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/os/freertos/services/osclock.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/os/freertos/services/osmutex.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/os/freertos/services/osoutput.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/os/freertos/services/ostask.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/os/freertos/services/ostimestamp.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/posix/path.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/posix/posix.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/tests/posix/fsstress.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/tests/util/atoi.c" $(REDHDR)
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/tests/util/math.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/tests/util/printf.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/tests/util/rand.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/tools/getopt.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/tools/toolcmn.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/util/bitmap.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/util/crc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/util/memory.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/util/namelen.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/util/sign.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/util/string.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/config/redconf.c"

        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/ks_component_base.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/ks_component_passive.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/ks_component_queued.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/ks_component_active.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/services/ks_timer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/services/ks_logger.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/src/components/services/ks_file_manager.cpp"

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
                "${CMAKE_CURRENT_SOURCE_DIR}/include/utilities"
                "${CMAKE_CURRENT_SOURCE_DIR}/include/drivers"

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
                "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/thirdparty/CMSIS/Include"
                "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/thirdparty/CMSIS/Lib/GCC"
                "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/services/clock"
                "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/utils/stdio/stdio_serial"
                "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils"
                "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils/preprocessor"
                "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/services/serial"
                "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/usart"
                "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/uart"

                "${CMAKE_CURRENT_SOURCE_DIR}/extern/RelianceEdge/include"
        )

        set(KRONOS_EXTRA_SRC "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils/syscalls/gcc/syscalls.c" PARENT_SCOPE)

target_include_directories(Kronos PUBLIC ${KRONOS_INCLUDE_DIRS})