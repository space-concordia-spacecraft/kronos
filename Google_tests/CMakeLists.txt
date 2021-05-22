project(Kronos_Tests)

add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Kronos_Tests_run KronosTests.cpp HashMapTests.cpp)

target_link_libraries(Kronos_Tests_run Kronos_lib)

target_link_libraries(Kronos_Tests_run gtest gtest_main)