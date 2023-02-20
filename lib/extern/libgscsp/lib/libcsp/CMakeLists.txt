file(GLOB LIBCSP_SRC
        "src/*.c"
        "src/arch/*.c"
        )

file(GLOB_RECURSE LIBCSP_SRC_REC
        "src/arch/freertos/**.c"
        "src/crypto/**.c"
        "src/external/**.c"
        "src/interfaces/**.c"
        "src/rtable/**.c"
        "src/transport/**.c"
        )
list(APPEND LIBCSP_SRC ${LIBCSP_SRC_REC})
list(APPEND LIBCSP_SRC "src/drivers/usart/usart_sam.c")

set(LIBCSP_INCLUDE_DIRS
        "include"
        "src")


add_mcu_library("libcsp" ${LIBCSP_SRC})
target_link_libraries("libcsp" PUBLIC asf libutil)
target_include_directories("libcsp" PUBLIC ${LIBCSP_INCLUDE_DIRS})