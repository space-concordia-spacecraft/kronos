file(GLOB_RECURSE NANOCAM_SRC
        "src/nanocam.c")

set(NANOCAM_INCLUDE_DIRS
        "include"
        "src")

add_mcu_library("nanocam" ${NANOCAM_SRC})
target_include_directories("nanocam" PUBLIC ${NANOCAM_INCLUDE_DIRS})
target_link_libraries("nanocam" PUBLIC libparam_client)
