file(GLOB LIBPARAM_CLIENT_SRC
        "src/*.c"
        )

file(GLOB_RECURSE LIBPARAM_CLIENT_SRC_REC
        "src/pp/**.c"
        "src/rparam/**.c"
        )
list(APPEND LIBPARAM_CLIENT_SRC ${LIBPARAM_CLIENT_SRC_REC})

set(LIBPARAM_CLIENT_INCLUDE_DIRS
        "include"
        "include/deprecated"
        "include/deprecated/param"
        "src")

add_mcu_library("libparam_client" ${LIBPARAM_CLIENT_SRC})
target_include_directories("libparam_client" PUBLIC ${LIBPARAM_CLIENT_INCLUDE_DIRS})
target_link_libraries("libparam_client" PUBLIC libgscsp)
