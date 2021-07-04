cmake_minimum_required(VERSION 3.17)

SET ( CMAKE_EXPORT_COMPILE_COMMANDS ON )
SET ( CMAKE_VERBOSE_MAKEFILE ON )

project(Kronos)

# Glob source files
file(GLOB SOURCE_FILES
        "extern/FreeRTOS/event_groups.c"
        "extern/FreeRTOS/portable/MemMang/heap_4.c"
        "extern/FreeRTOS/list.c"
        "extern/FreeRTOS/portable/GCC/ARM_CM7/r0p1/port.c"
        "extern/FreeRTOS/queue.c"
        "extern/FreeRTOS/tasks.c"
        "extern/FreeRTOS/timers.c"
        "extern/Common/Minimal/BlockQ.c"
        "extern/Common/Minimal/death.c"
        "extern/Common/Minimal/blocktim.c"
        "extern/Common/Minimal/countsem.c"
        "extern/Common/Minimal/dynamic.c"
        "extern/Common/Minimal/EventGroupsDemo.c"
        "extern/Common/Minimal/GenQTest.c"
        "extern/Common/Minimal/flop.c"
        "extern/Common/Minimal/IntQueue.c"
        "extern/Common/Minimal/IntSemTest.c"
        "extern/Common/Minimal/QueueOverwrite.c"
        "extern/Common/Minimal/recmutex.c"
        "extern/Common/Minimal/TaskNotify.c"
        "extern/Common/Minimal/semtest.c"
        "extern/Common/Minimal/TimerDemo.c"
        "extern/ASF/common/services/clock/same70/sysclk.c"
        "extern/ASF/common/utils/interrupt/interrupt_sam_nvic.c"
        "extern/ASF/common/services/serial/usart_serial.c"
        "extern/ASF/common/utils/stdio/read.c"
        "extern/ASF/common/utils/stdio/write.c"
        "extern/ASF/sam/boards/same70_xplained/init.c"
        "extern/ASF/sam/drivers/matrix/matrix.c"
        "extern/ASF/sam/drivers/mpu/mpu.c"
        "extern/ASF/sam/drivers/pio/pio.c"
        "extern/ASF/sam/drivers/pio/pio_handler.c"
        "extern/ASF/sam/drivers/pmc/pmc.c"
        "extern/ASF/sam/drivers/pmc/sleep.c"
        "extern/ASF/sam/drivers/tc/tc.c"
        "extern/ASF/sam/drivers/uart/uart.c"
        "extern/ASF/sam/drivers/usart/usart.c"
        "extern/ASF/sam/utils/syscalls/gcc/syscalls.c"
        "extern/ASF/sam/utils/cmsis/same70/source/templates/system_same70.c"
        "extern/ASF/sam/utils/cmsis/same70/source/templates/gcc/startup_same70.c"
        "src/Blinky_Demo/main_blinky.c"
        "src/main.cpp"
        "src/ks_bus.cpp"
        "src/components/ks_component_passive.cpp"
        "src/components/ks_component_queued.cpp"
        "src/components/ks_component_active.cpp")

add_sam_executable(Kronos ${SOURCE_FILES} )

target_include_directories(Kronos PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/types"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/components"

        "${CMAKE_CURRENT_SOURCE_DIR}/src/Full_Demo"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/FreeRTOS/portable/GCC/ARM_CM7/r0p1"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/config"

#        "${CMAKE_CURRENT_SOURCE_DIR}/../sam/applications/getting-started/same70q21_same70_xplained"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/utils"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/boards/same70_xplained"
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
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils/cmsis/same70/source/templates"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/utils/stdio/stdio_serial"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils/preprocessor"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/common/services/serial"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/usart"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/drivers/uart"
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/ASF/sam/utils/cmsis/same70/include"

        "${CMAKE_CURRENT_SOURCE_DIR}/extern/Common/include")