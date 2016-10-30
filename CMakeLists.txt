cmake_minimum_required(VERSION 3.2)
project(LiveOV7670)


# PlatformIO build targets
# Must init platformio. See: PlatformIO_CLion_init.txt
add_subdirectory(arduino)



# init Google Test target
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# include fake arduino definitsion and functions
include_directories(test/fake)
# include module to be tested
add_subdirectory(arduino/src)
# include tests
add_subdirectory(test)



