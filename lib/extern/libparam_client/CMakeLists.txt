set(LIBPARAM_CLIENT_SRC
        "src/serialize.c"
        "src/string.c"
        "src/table.c"
        "src/rparam/deprecated_rparam.c"
        "src/rparam/rparam.c"
        "src/rparam/query.c"
        "src/rparam/cmd/cmd_rparam.c"

        "src/pp/pp.c"
        "src/pp/cmd/master_cmd.c"
        "src/pp/i2c/i2c.c"
        "src/pp/spi/spi.c"

        )

set(LIBPARAM_CLIENT_INCLUDE_DIRS
        "include"
        "include/deprecated"
        "src")

add_mcu_library("libparam_client" ${LIBPARAM_CLIENT_SRC})
target_include_directories("libparam_client" PUBLIC ${LIBPARAM_CLIENT_INCLUDE_DIRS})
target_link_libraries("libparam_client" PUBLIC libgscsp)
