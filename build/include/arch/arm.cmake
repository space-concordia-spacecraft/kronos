set(ARM_FLAGS
        # Specify CPU (Central Processing Unit)
        "-mcpu=${ARM_CPU}"
        # Specify FPU (Floating Point Unit)
        "-mfpu=${ARM_FPU}"
        # Specify floating point ABI (Application Binary Interface)
        "-mfloat-abi=${ARM_FLOAT_ABI}")

if(DEFINED ARM_LONG_CALLS)
    # Use long calls (32-bit offsets instead of 24-bit offsets)
    list(APPEND ARM_FLAGS "-mlong-calls")
endif()

if(DEFINED ARM_THUMB)
    # Use Thumb instruction set
    list(APPEND ARM_FLAGS "-mthumb")
endif()

set(ARM_COMPILE_FLAGS ${ARM_FLAGS})

set(ARM_LINK_FLAGS ${ARM_FLAGS})

if(DEFINED ARM_CM7_MATH)
    # Enable CM7 math instructions
    list(APPEND ARM_COMPILE_DEFINITIONS ARM_MATH_CM7=true)
endif()
