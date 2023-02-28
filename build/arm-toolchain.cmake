set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

function(restore_cache_variable NAME)
    if(${NAME})
        set("ENV{_${NAME}}" "${${NAME}}")
    else()
        set("${NAME}" "$ENV{_${NAME}}" PARENT_SCOPE)
    endif()
endfunction()

restore_cache_variable(KS_TOOLCHAIN_DIR)
restore_cache_variable(KS_BOARD)
restore_cache_variable(KS_ARCH)

include("${KS_TOOLCHAIN_DIR}/toolchain.cmake")