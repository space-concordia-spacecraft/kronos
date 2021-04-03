# CMAKE minimum version
cmake_minimum_required(VERSION 3.17)

# Setup project
project("kronos")

# Clump source files from src
file(GLOB KRONOS_SRC "src/**/*.cpp" "src/**/*.c" "src/*.cpp" "src/*.c")

# Setup binary
add_sam_executable("kronos" ${KRONOS_SRC})

# Extra target linking
target_include_directories("kronos" PRIVATE "include")