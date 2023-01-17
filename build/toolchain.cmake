set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> <LINK_FLAGS> -c -r -o <TARGET> <OBJECTS>")
set(CMAKE_C_ARCHIVE_APPEND "<CMAKE_AR> <LINK_FLAGS> -r -o <TARGET> <OBJECTS>")
set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> <LINK_FLAGS> -c -r -o <TARGET> <OBJECTS>")
set(CMAKE_CXX_ARCHIVE_APPEND "<CMAKE_AR> <LINK_FLAGS> -r -o <TARGET> <OBJECTS>")

find_program(CMAKE_OBJCOPY arm-none-eabi-objcopy)

# Check board and architecture
if(NOT KS_BOARD)
    message(FATAL_ERROR "KS_BOARD is not defined! Set the variable before configuring cmake with this toolchain!")
endif()

if(NOT KS_ARCH)
    message(FATAL_ERROR "KS_ARCH is not defined! Set the variable before configuring cmake with this toolchain!")
endif()

# Include board and architecture files
include("${PROJECT_SOURCE_DIR}/build/include/board/${KS_BOARD}.cmake")
include("${PROJECT_SOURCE_DIR}/build/include/arch/${KS_ARCH}.cmake")

string(REPLACE ";" " " KS_COMPILE_FLAGS "${KS_COMPILE_FLAGS}")
string(REPLACE ";" " " KS_LINK_FLAGS "${KS_LINK_FLAGS}")

message("KS_COMPILE_FLAGS: ${KS_COMPILE_FLAGS}")
message("KS_LINK_FLAGS: ${KS_LINK_FLAGS}")
message("KS_COMPILE_DEFINITIONS: ${KS_COMPILE_DEFINITIONS}")

function(print_sources TARGET)
    set(SOURCES ${ARGN})
    list(LENGTH SOURCES NUM_SOURCES)
    if(NUM_SOURCES LESS 0)
        message(FATAL_ERROR "No source files provided for ${TARGET}")
    else()
        foreach(SRC ${SOURCES})
            message(STATUS "Including source: ${SRC}")
        endforeach()
    endif()
    message("")
endfunction()

function(add_mcu_executable EXECUTABLE_NAME)
    # Print sources
    message(STATUS "Creating executable '${EXECUTABLE_NAME}'")
    set(SOURCES ${ARGN})
    print_sources(EXECUTABLE_NAME ${SOURCES})

    # Output files
    set(ELF_OUTPUT_FILE "${EXECUTABLE_NAME}.elf")
    set(BIN_OUTPUT_FILE "${EXECUTABLE_NAME}.bin")
    set(MAP_OUTPUT_FILE "${EXECUTABLE_NAME}.map")

    # Create executable target
    add_executable(${EXECUTABLE_NAME} ${SOURCES})
    set_target_properties(
            ${EXECUTABLE_NAME}
            PROPERTIES
            OUTPUT_NAME "${ELF_OUTPUT_FILE}"
            COMPILE_FLAGS "-O0 -g3 -Wall -Wextra -c -MD -MP ${KS_COMPILE_FLAGS}"
            LINK_FLAGS "-g3 -Wl,-Map=\"${MAP_OUTPUT_FILE}\" ${KS_LINK_FLAGS}"
    )
    message("KS_COMPILE_DEFINITIONS: ${KS_COMPILE_DEFINITIONS}")
    target_compile_definitions(${EXECUTABLE_NAME} PUBLIC ${KS_COMPILE_DEFINITIONS})

    # Create binary from elf file
    add_custom_target(
            ${BIN_OUTPUT_FILE}
            ${CMAKE_OBJCOPY} -O binary ./${ELF_OUTPUT_FILE} ./${BIN_OUTPUT_FILE}
            DEPENDS ${EXECUTABLE_NAME}
    )

    # Clean map file
    set_directory_properties(
            PROPERTIES
            ADDITIONAL_MAKE_CLEAN_FILES "${ADDITIONAL_MAKE_CLEAN_FILES} ${MAP_OUTPUT_FILE}"
    )
endfunction(add_mcu_executable)

function(add_mcu_library LIBRARY_NAME)
    # Print sources
    message(STATUS "Creating library '${LIBRARY_NAME}'")
    set(SOURCES ${ARGN})
    print_sources(LIBRARY_NAME ${SOURCES})

    # Create static library.
    add_library(${LIBRARY_NAME} STATIC ${SOURCES})
    set_target_properties(
            ${LIBRARY_NAME}
            PROPERTIES
            COMPILE_FLAGS "-O0 -Wall -Wextra -c -MD -MP ${KS_COMPILE_FLAGS}"
            ARCHIVE_OUTPUT_NAME "${LIBRARY_NAME}"
    )
    message("KS_COMPILE_DEFINITIONS: ${KS_COMPILE_DEFINITIONS}")
    target_compile_definitions(${LIBRARY_NAME} PUBLIC ${KS_COMPILE_DEFINITIONS})
endfunction(add_mcu_library)