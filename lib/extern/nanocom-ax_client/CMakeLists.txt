file(GLOB_RECURSE NANOCOM_SRC
        "src/ax100_client.c")

set(NANOCOM_INCLUDE_DIRS
        "include"
        "src")

add_mcu_library("nanocom" ${NANOCOM_SRC})
target_include_directories("nanocom" PUBLIC ${NANOCOM_INCLUDE_DIRS})
target_link_libraries("nanocom" PUBLIC libparam_client)
