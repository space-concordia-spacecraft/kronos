INCLUDE(CMakeForceCompiler)

SET ( CMAKE_SYSTEM_NAME Generic )
SET ( CMAKE_SYSTEM_PROCESSOR arm )

SET ( CMAKE_SYSTEM_VERSION 1 )

SET ( PACKS_REPO "C:\\Program Files (x86)\\Atmel\\Studio\\7.0\\packs")

SET ( CMAKE_EXPORT_COMPILE_COMMANDS ON )
SET ( CMAKE_VERBOSE_MAKEFILE ON )

#
#	ARM GCC Toolchain Configuration
#
SET ( CMAKE_C_COMPILER_WORKS 1 )
SET ( CMAKE_CXX_COMPILER_WORKS 1 )
SET ( CMAKE_C_COMPILER arm-none-eabi-gcc )
SET ( CMAKE_CXX_COMPILER arm-none-eabi-g++ )
SET ( CMAKE_OBJCOPY arm-none-eabi-objcopy )
SET ( CMAKE_LINKER arm-none-eabi-ld )
SET ( CMAKE_SIZE arm-none-eabi-size )
SET ( CMAKE_C_STANDARD 99 )
SET ( CMAKE_CXX_STANDARD 17 )

# C and C++ specific flags
#SET ( CMAKE_C_FLAGS "-x c -std=gnu99" )
#SET ( CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions" )

# ARM GCC Toolchain Folder
if(NOT ARM_TOOLCHAIN_PATH)
    set (
            ARM_TOOLCHAIN_PATH "C:\\Program Files (x86)\\Atmel\\Studio\\7.0\\toolchain\\arm\\arm-gnu-toolchain\\bin"
            CACHE STRING "Set default path: C:\\Program Files (x86)\\Atmel\\Studio\\7.0\\toolchain\\arm\\arm-gnu-toolchain\\bin"
    )
endif()

# CPU
if (NOT ARM_CPU)
    set (
            ARM_CPU cortex-m7
            CACHE STRING "Set default MCU: cortex-m7 (see 'arm-none-eabi-gcc --target-help' for valid values)"
    )
endif()

# Architecture
if (NOT ATMEL_ARCH)
    set (
            ATMEL_ARCH SAME70
            CACHE STRING "Set the default architecture: SAME70"
    )
endif()

# Microcontroller
if (NOT SAM_MCU)
    set (
            SAM_MCU same70q21
            CACHE STRING "Set the default MCU platform: same70q21"
    )
endif()

string(TOUPPER "${SAM_MCU}" SAM_MCU_UPPER)

# Board
if (NOT SAM_BOARD)
    set (
            SAM_BOARD SAME70_XPLAINED
            CACHE STRING "Set the default MCU platform: SAME70_XPLAINED"
    )
endif()

#
#	Bossac Specific
#
if (USE_UPLOADER)
    if (NOT ARM_UPLOADTOOL)
        SET (
                ARM_UPLOADTOOL bossac
                CACHE STRING "Set default upload tool: bossac"
        )
        find_program(ARM_UPLOADTOOL bossac)
    endif(NOT ARM_UPLOADTOOL)


    if (NOT UPLOAD_PORT)
        SET (
                UPLOAD_PORT COM6
                CACHE STRING "Set upload port: COM<port_num>"
        )
    endif(NOT UPLOAD_PORT)
endif(USE_UPLOADER)


#
#	UF2 Specific
#
if (USE_UF2_BOOTLOADER)
    if (NOT SHELL_COPY_COMMAND)
        SET (
                SHELL_COPY_COMMAND copy
                CACHE STRING "Set shell copy command: default 'copy'"
        )
    endif(NOT SHELL_COPY_COMMAND)


    if (NOT UF2_DRIVE_MOUNT)
        SET (
                UF2_DRIVE_MOUNT "D:"
                CACHE STRING "Set the mount point the UF2 populates: default 'D:'"
        )
    endif(NOT UF2_DRIVE_MOUNT)
endif (USE_UF2_BOOTLOADER)

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH  "C:\\Program Files (x86)\\Atmel\\Studio\\7.0\\toolchain\\arm\\arm-gnu-toolchain")

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Debug/Release definition
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(BUILD_TYPE "NDEBUG")
else()
    set(BUILD_TYPE "DEBUG")
endif()


##
# FUNCTION get_file_language
#
#   PARAM SRC_FILE The path of the source file
#
function (set_compile_flags SRC_FILE)
    get_filename_component(SRC_EXT ${SRC_FILE} EXT)

    # SRC_EXT includes the .
    # The following regex removes the . so that only the language remains. For example: c, cpp
    string(REGEX REPLACE "\\." "" SRC_EXT ${SRC_EXT})

    # Set language to C
    if (${SRC_EXT} IN_LIST CMAKE_C_SOURCE_FILE_EXTENSIONS)
        set_source_files_properties(${SRC_FILE} PROPERTIES COMPILE_FLAGS "-x c")
        return()
    endif()

endfunction (set_compile_flags)
##
#	FUNCTION add_sam_executable
#
#	PARAM EXECUTABLE_NAME The name of the executable.
#	ARGN Any source files required by the executable.
#
function (add_sam_executable EXECUTABLE_NAME)

    set (additional_source_files ${ARGN})
    list(LENGTH additional_source_files num_of_source_files)

    if (num_of_source_files LESS 0)
        message(FATAL_ERROR "No source files provided for ${EXECUTABLE_NAME}")
    else()
        foreach(src_file ${additional_source_files})
            message (STATUS "Including source: ${src_file}")
            set_compile_flags(${src_file} SRC_LANG)
        endforeach()
    endif()

    set (ELF_OUTPUT_FILE "${EXECUTABLE_NAME}.elf")
    set (BIN_OUTPUT_FILE "${EXECUTABLE_NAME}.bin")
    set (UF2_OUTPUT_FILE "${EXECUTABLE_NAME}.uf2")
    #set (HEX_OUTPUT_FILE "${EXECUTABLE_NAME}.hex")
    #set (LST_OUTPUT_FILE "${EXECUTABLE_NAME}.lst")
    set (MAP_OUTPUT_FILE "${EXECUTABLE_NAME}.map")
    #set (EEPROM_IMAGE "${EXECUTABLE_NAME}-eeprom.hex")

    ##
    #	Create ELF.
    add_executable(${EXECUTABLE_NAME} EXCLUDE_FROM_ALL ${additional_source_files})
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES OUTPUT_NAME "${ELF_OUTPUT_FILE}")

    target_include_directories(${EXECUTABLE_NAME} PUBLIC "${PACKS_REPO}/arm/CMSIS/5.4.0/CMSIS/Core/Include")
    set_target_properties (
            ${EXECUTABLE_NAME}
            PROPERTIES
            COMPILE_FLAGS "-mthumb -O0 -fdata-sections -ffunction-sections -mlong-calls -g3 -Wall -Wextra -mcpu=${ARM_CPU} -c -pipe -fno-strict-aliasing -ffunction-sections -fdata-sections --param max-inline-insns-single=500 -mfloat-abi=softfp -mfpu=fpv5-sp-d16 -MD -MP"
            LINK_FLAGS "-mthumb -Wl,-Map=\"${EXECUTABLE_NAME}.map\" -Wl,--start-group -lm  -Wl,--end-group -L\"${CMAKE_SOURCE_DIR}/../scripts/gcc\"  -Wl,--gc-sections -mcpu=${ARM_CPU} -T ${SAM_MCU}_flash.ld"
    )	# TODO: Hardcoded -> DEBUG, __SAMD51J20A__

    target_compile_definitions(${EXECUTABLE_NAME} PUBLIC ${BUILD_TYPE} "__${SAM_MCU_UPPER}__" "BOARD=${SAM_BOARD}" "scanf=iscanf" "ARM_MATH_CM7=true" "printf=iprintf")

    ##
    #	Create binary from ELF.
    add_custom_target (
            ${BIN_OUTPUT_FILE}
            "${ARM_TOOLCHAIN_PATH}/${CMAKE_OBJCOPY}" -O binary ./${ELF_OUTPUT_FILE} ./${BIN_OUTPUT_FILE}
            DEPENDS ${EXECUTABLE_NAME}
    )

    get_directory_property (
            clean_files ADDITIONAL_MAKE_CLEAN_FILES
    )
    set_directory_properties (
            PROPERTIES
            ADDITIONAL_MAKE_CLEAN_FILES "${MAP_OUTPUT_FILE}"
    )


    if (USE_UPLOADER)
        ##
        #	Create utility project for uploading via BOSSA
        add_custom_target (
                "Upload_${EXECUTABLE_NAME}"
                ${ARM_UPLOADTOOL} -i -d --port=${UPLOAD_PORT} -U --offset=0x4000 -w -v ./${BIN_OUTPUT_FILE} -R
                DEPENDS ${BIN_OUTPUT_FILE}
        )

        set_target_properties (
                "Upload_${EXECUTABLE_NAME}"
                PROPERTIES
                FOLDER "deploy"
        )
    endif (USE_UPLOADER)


    if (USE_UF2_BOOTLOADER)
        ##
        #	Create the UF2 file.
        add_custom_target (
                ${UF2_OUTPUT_FILE}
                python ${CMAKE_SOURCE_DIR}/scripts/uf2/uf2conv.py -c -b 0x4000 -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${UF2_OUTPUT_FILE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BIN_OUTPUT_FILE}
                DEPENDS ${BIN_OUTPUT_FILE}
        )

        ##
        #	Copy UF2 file to the MSC drive.
        add_custom_target (
                "Flash_${UF2_OUTPUT_FILE}"
                ${SHELL_COPY_COMMAND} bin\\${UF2_OUTPUT_FILE} ${UF2_DRIVE_MOUNT}
                VERBATIM
                USES_TERMINAL
                DEPENDS ${UF2_OUTPUT_FILE}
        )

        set_target_properties (
                "Flash_${UF2_OUTPUT_FILE}"
                PROPERTIES
                FOLDER "deploy"
        )
    endif(USE_UF2_BOOTLOADER)

endfunction(add_sam_executable)


##
#
function (add_sam_library LIBRARY_NAME)

    set (additional_source_files ${ARGN})
    list(LENGTH additional_source_files num_of_source_files)

    if (num_of_source_files LESS 0)
        message(FATAL_ERROR "No source files provided for ${LIBRARY_NAME}")
    else()
        foreach(src_file ${additional_source_files})
            message (STATUS "Including source: ${src_file}")
        endforeach()
    endif()

    set (STATICLIB_OUTPUT_FILE "${LIBRARY_NAME}.a")


    ##
    #	Create static library.
    add_library(${LIBRARY_NAME} STATIC EXCLUDE_FROM_ALL ${additional_source_files})

    target_include_directories(${LIBRARY_NAME} PUBLIC "C:/ProgramData/Atmel/AtmelStudio/7.0/packs/arm/CMSIS/5.4.0/CMSIS/Core/Include" PUBLIC "C:/ProgramData/Atmel/AtmelStudio/7.0/packs/atmel/SAMD51_DFP/1.2.139/samd51a/include"  )
    set_target_properties (
            ${LIBRARY_NAME}
            PROPERTIES
            COMPILE_FLAGS "-mthumb -O1 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=${ARM_CPU} -c -std=gnu99 -MD -MP -MF \"${LIBRARY_NAME}.d\"" #-MT\"library.d\" -MT\"library.o\"   -o \"library.o\" \".././library.c\""
            LINKER_LANGUAGE "C"
            ARCHIVE_OUTPUT_NAME "${LIBRARY_NAME}"
    ) # TODO: __SAMD51J20A__ is hardcoded!

    target_compile_definitions(${LIBRARY_NAME} PUBLIC ${BUILD_TYPE} "__${SAM_MCU_UPPER}__" "BOARD=${SAM_BOARD}")

endfunction(add_sam_library)