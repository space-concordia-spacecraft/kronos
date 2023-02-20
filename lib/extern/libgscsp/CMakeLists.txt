add_subdirectory("lib/libcsp")

file(GLOB LIBGSCSP_SRC
        "src/*.c"
        )

file(GLOB_RECURSE LIBGSCSP_SRC_REC
        "src/drivers/kiss/**.c"
        )
list(APPEND LIBGSCSP_SRC ${LIBGSCSP_SRC_REC})

set(LIBGSCSP_INCLUDE_DIRS
        "include"
        "src")

add_mcu_library("libgscsp" ${LIBGSCSP_SRC})
target_include_directories("libgscsp" PUBLIC ${LIBGSCSP_INCLUDE_DIRS})
target_link_libraries("libgscsp" PUBLIC libcsp)
