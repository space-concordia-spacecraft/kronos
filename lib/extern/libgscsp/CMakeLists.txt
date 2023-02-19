add_subdirectory("lib/libcsp")

set(LIBGSCSP_SRC
        "src/clock.c"
        "src/commands.c"
        "src/conn.c"
        "src/csp.c"
        "src/error.c"
        "src/log.c"
        "src/router.c"
        "src/rtable.c"
        "src/service_dispatcher.c"
        "src/service_handler.c"
        "src/transaction.c"
        )

set(LIBGSCSP_INCLUDE_DIRS
        "include"
        "src")

add_mcu_library("libgscsp" ${LIBGSCSP_SRC})
target_include_directories("libgscsp" PUBLIC ${LIBGSCSP_INCLUDE_DIRS})
target_link_libraries("libgscsp" PUBLIC libutil libcsp)
