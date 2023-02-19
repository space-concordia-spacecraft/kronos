include("${PROJECT_SOURCE_DIR}/build/include/arch/arm.cmake")

string(TOUPPER "${SAM_MCU}" SAM_MCU_UPPER)
string(TOUPPER "${SAM_BOARD}" SAM_BOARD_UPPER)

set(KS_COMPILE_FLAGS
        # Machine options
        ${ARM_COMPILE_FLAGS}
        # Language Options
        -fdata-sections
        -ffunction-sections
        -fno-strict-aliasing
        --param max-inline-insns-single=500)

set(KS_LINK_FLAGS
        # Machine options
        ${ARM_LINK_FLAGS}
        # Math library: libm.a
        -Wl,--start-group -lm -Wl,--end-group
        # Remove unused code
        -Wl,--gc-sections
        # Entry point
        -Wl,--entry=Reset_Handler
        # Generate cross-reference table
        -Wl,--cref
        # Linker script
        -T \"${CMAKE_SOURCE_DIR}/lib/extern/asf/${SAM_ARCH}/${SAM_ARCH}/gcc/gcc/${SAM_MCU}_flash.ld\")

set(KS_COMPILE_DEFINITIONS
        # ASF + FreeRTOS
        KS_FREERTOS_API
        KS_ASF_DRIVERS
        # Machine options
        ${ARM_COMPILE_DEFINITIONS}
        # Microcontroller
        "__${SAM_MCU_UPPER}__"
        # Board
        "BOARD=${SAM_BOARD_UPPER}")

set(KS_MCU ${SAM_MCU})