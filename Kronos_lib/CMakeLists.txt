project(Kronos)

#set(HEADER_FILES
#        include/bus.h
#        )

#set(SOURCE_FILES
#        src/bus.cpp
#        )

file(GLOB SOURCE_FILES "src/**/*.cpp" "src/**/*.c" "src/*.cpp" "src/*.c")
file(GLOB HEADER_FILES "include/*.hpp" "include/*.h" "include/**/*.h" "include/**/*.hpp")

add_library(Kronos_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})

include_directories(Kronos_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/include/types")