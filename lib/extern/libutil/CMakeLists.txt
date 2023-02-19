file(GLOB LIBUTIL_SRC
        "src/*.c"
        "src/cmd/*.c"
        )

file(GLOB_RECURSE LIBUTIL_SRC_REC
        "src/drivers/**.c"
        "src/gosh/**.c"
        "src/log/**.c"
        "src/vmem/**.c"
        "src/watchdog/**.c"
        "src/zip/**.c"
        )
list(APPEND LIBUTIL_SRC ${LIBUTIL_SRC_REC})

set(LIBUTIL_INCLUDE_DIRS
        "include"
        "src")

add_mcu_library("libutil" ${LIBUTIL_SRC})
target_include_directories("libutil" PUBLIC ${LIBUTIL_INCLUDE_DIRS})
target_link_libraries("libutil" PUBLIC freertos_kernel)
