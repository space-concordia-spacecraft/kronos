include(CMakeForceCompiler)

#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_VERBOSE_MAKEFILE ON)

# ARM GCC Toolchain Configuration
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> <LINK_FLAGS> -c -r -o <TARGET> <OBJECTS>")
set(CMAKE_C_ARCHIVE_APPEND "<CMAKE_AR> <LINK_FLAGS> -r -o <TARGET> <OBJECTS>")
set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> <LINK_FLAGS> -c -r -o <TARGET> <OBJECTS>")
set(CMAKE_CXX_ARCHIVE_APPEND "<CMAKE_AR> <LINK_FLAGS> -r -o <TARGET> <OBJECTS>")

find_program(CMAKE_OBJCOPY arm-none-eabi-objcopy)
get_filename_component(CMAKE_TOOLCHAIN_DIR "${CMAKE_TOOLCHAIN_FILE}" DIRECTORY)

# CPU
if (NOT ARM_CPU)
    set(
            ARM_CPU cortex-m7
            CACHE STRING "Set default MCU: cortex-m7 (see 'arm-none-eabi-gcc --target-help' for valid values)"
    )
endif ()

# Architecture
if (NOT SAM_ARCH)
    set(
            SAM_ARCH same70
            CACHE STRING "Set the default architecture: same70"
    )
endif ()

# CPU
if (NOT SAM_MCU)
    set(
            SAM_MCU same70q21
            CACHE STRING "Set the default MCU platform: same70q21"
    )
endif ()

string(TOUPPER "${SAM_MCU}" SAM_MCU_UPPER)

# Board
if (NOT SAM_BOARD)
    set(
            SAM_BOARD same70_xplained
            CACHE STRING "Set the default MCU platform: same70_xplained"
    )
endif ()

string(TOUPPER "${SAM_BOARD}" SAM_BOARD_UPPER)

# Bossac upload tool
if (USE_UPLOADER)
    if (NOT SAM_UPLOAD_TOOL)
        set(
                SAM_UPLOAD_TOOL bossac
                CACHE STRING "Set default upload tool: bossac"
        )
        find_program(SAM_UPLOAD_TOOL bossac)
    endif (NOT SAM_UPLOAD_TOOL)
endif (USE_UPLOADER)


# where is the target environment
SET(CMAKE_FIND_ROOT_PATH "C:\\Program Files (x86)\\Atmel\\Studio\\7.0\\toolchain\\arm\\arm-gnu-toolchain")

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Debug/Release definition
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(BUILD_TYPE "NDEBUG")
else ()
    set(BUILD_TYPE "DEBUG")
endif ()


##
# FUNCTION get_file_language
#
#   PARAM SRC_FILE The path of the source file
#
function(set_compile_flags SRC_FILE)
    get_filename_component(SRC_EXT ${SRC_FILE} EXT)

    # SRC_EXT includes the .
    # The following regex removes the . so that only the language remains. For example: c, cpp
    string(REGEX REPLACE "\\." "" SRC_EXT ${SRC_EXT})

    # Set language to C
    if (${SRC_EXT} IN_LIST CMAKE_C_SOURCE_FILE_EXTENSIONS)
        set_source_files_properties(${SRC_FILE} PROPERTIES COMPILE_FLAGS "-x c")
        return()
    endif ()

    # Set language to C++
    if (${SRC_EXT} IN_LIST CMAKE_CXX_SOURCE_FILE_EXTENSIONS)
        set_source_files_properties(${SRC_FILE} PROPERTIES COMPILE_FLAGS "-fno-rtti")
        return()
    endif ()

endfunction(set_compile_flags)


##
#	FUNCTION add_sam_executable
#
#	PARAM EXECUTABLE_NAME The name of the executable.
#	ARGN Any source files required by the executable.
#
function(add_sam_executable EXECUTABLE_NAME)

    set(additional_source_files ${ARGN})
    list(LENGTH additional_source_files num_of_source_files)

    message(STATUS "Creating SAM executable '${EXECUTABLE_NAME}'")
    if (num_of_source_files LESS 0)
        message(FATAL_ERROR "No source files provided for ${EXECUTABLE_NAME}")
    else ()
        foreach (src_file ${additional_source_files})
            message(STATUS "Including source: ${src_file}")
            set_compile_flags(${src_file} SRC_LANG)
        endforeach ()
    endif ()
    message("")

    set(ELF_OUTPUT_FILE "${EXECUTABLE_NAME}.elf")
    set(BIN_OUTPUT_FILE "${EXECUTABLE_NAME}.bin")
    set(UF2_OUTPUT_FILE "${EXECUTABLE_NAME}.uf2")
    #set (HEX_OUTPUT_FILE "${EXECUTABLE_NAME}.hex")
    #set (LST_OUTPUT_FILE "${EXECUTABLE_NAME}.lst")
    set(MAP_OUTPUT_FILE "${EXECUTABLE_NAME}.map")
    #set (EEPROM_IMAGE "${EXECUTABLE_NAME}-eeprom.hex")

    ##
    #	Create ELF.
    add_executable(${EXECUTABLE_NAME} EXCLUDE_FROM_ALL ${additional_source_files})
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES OUTPUT_NAME "${ELF_OUTPUT_FILE}")

    set_target_properties(
            ${EXECUTABLE_NAME}
            PROPERTIES
            COMPILE_FLAGS "-mthumb -O0 -fdata-sections -ffunction-sections -mlong-calls -g3 -Wall -Wextra -Wno-expansion-to-defined -Wno-unused-parameter -mcpu=${ARM_CPU} -c -pipe -fno-strict-aliasing --param max-inline-insns-single=500 -mfloat-abi=softfp -mfpu=fpv5-sp-d16 -MD -MP"
            LINK_FLAGS "-mthumb -g3 -Wl,-Map=\"${EXECUTABLE_NAME}.map\" -Wl,--start-group -lm  -Wl,--end-group -L\"${CMAKE_TOOLCHAIN_DIR}/scripts/gcc\"  -Wl,--gc-sections -mcpu=${ARM_CPU} -Wl,--entry=Reset_Handler -Wl,--cref -T ${CMAKE_CURRENT_SOURCE_DIR}/../lib/build/scripts/gcc/${SAM_MCU}_flash.ld"
    )

    target_compile_definitions(${EXECUTABLE_NAME} PUBLIC ${BUILD_TYPE} "__${SAM_MCU_UPPER}__" "BOARD=${SAM_BOARD_UPPER}" "scanf=iscanf" "ARM_MATH_CM7=true" "printf=iprintf")

    ##
    #	Create binary from ELF.
    add_custom_target(
            ${BIN_OUTPUT_FILE}
            ${CMAKE_OBJCOPY} -O binary ./${ELF_OUTPUT_FILE} ./${BIN_OUTPUT_FILE}
            DEPENDS ${EXECUTABLE_NAME}
    )

    get_directory_property(
            clean_files ADDITIONAL_MAKE_CLEAN_FILES
    )
    set_directory_properties(
            PROPERTIES
            ADDITIONAL_MAKE_CLEAN_FILES "${MAP_OUTPUT_FILE}"
    )

    if (USE_UPLOADER)
        ##
        #	Create utility project for uploading via BOSSA
        add_custom_target(
                "Upload_${EXECUTABLE_NAME}"
                ${SAM_UPLOAD_TOOL} -e -w -v -b ./Kronos.bin
                DEPENDS ${BIN_OUTPUT_FILE}
        )

        set_target_properties(
                "Upload_${EXECUTABLE_NAME}"
                PROPERTIES
                FOLDER "deploy"
        )
    endif (USE_UPLOADER)

endfunction(add_sam_executable)


##
#
function(add_sam_library LIBRARY_NAME)

    set(additional_source_files ${ARGN})
    list(LENGTH additional_source_files num_of_source_files)

    message(STATUS "Creating SAM library '${LIBRARY_NAME}'")
    if (num_of_source_files LESS 0)
        message(FATAL_ERROR "No source files provided for ${LIBRARY_NAME}")
    else ()
        foreach (src_file ${additional_source_files})
            message(STATUS "Including source: ${src_file}")
            set_compile_flags(${src_file} SRC_LANG)
        endforeach ()
    endif ()
    message("")

    set(STATICLIB_OUTPUT_FILE "${LIBRARY_NAME}.a")


    # Create static library.
    add_library(${LIBRARY_NAME} STATIC EXCLUDE_FROM_ALL ${additional_source_files})
    set_target_properties(
            ${LIBRARY_NAME}
            PROPERTIES
            COMPILE_FLAGS "-mthumb -O0 -fdata-sections -ffunction-sections -mlong-calls -Wall -Wextra -Wno-expansion-to-defined -Wno-unused-parameter -mcpu=${ARM_CPU} -c --param max-inline-insns-single=500 -mfloat-abi=softfp -mfpu=fpv5-sp-d16 -MD -MP"
            ARCHIVE_OUTPUT_NAME "${LIBRARY_NAME}"
    )

    target_compile_definitions(${LIBRARY_NAME} PUBLIC ${BUILD_TYPE} "__${SAM_MCU_UPPER}__" "BOARD=${SAM_BOARD_UPPER}" "scanf=iscanf" "ARM_MATH_CM7=true" "printf=iprintf")

endfunction(add_sam_library)