cmake_minimum_required(VERSION 3.2)
project(LiveOV7670_tests)



#include_directories(fake)
add_subdirectory(lib/gtest-1.7.0)
add_subdirectory(fake)
include_directories(${LiveOV7670_src})
include_directories(${gtest_SOURCE_DIR}/include)


add_executable(runTests
        src/basic_test.cpp
        src/basic_test2.cpp)

target_link_libraries(runTests gtest_main)
target_link_libraries(runTests LiveOV7670_camera)
target_link_libraries(runTests fakeArduino)



