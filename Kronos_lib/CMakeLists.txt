project(Kronos)

# Glob source files
file(GLOB SOURCE_FILES "src/**/*.cpp" "src/**/*.c" "src/*.cpp" "src/*.c" "src/**/*.h" "src/*.h")

add_library(Kronos_lib STATIC ${SOURCE_FILES})

include_directories(Kronos_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/include/types")