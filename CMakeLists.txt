# CMAKE minimum version
cmake_minimum_required(VERSION 3.17)

# Setup project
project("kronos")

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Clump source files from src
file(GLOB KRONOS_SRC "src/**/*.cpp" "src/**/*.c" "src/*.cpp" "src/*.c")

# Specify include directories
include_directories(src/include)

# Setup binary
add_sam_executable("kronos" ${KRONOS_SRC})

# Extra target linking
target_include_directories("kronos" src/include)