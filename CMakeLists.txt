cmake_minimum_required(VERSION 3.2)
project(LiveOV7670)




add_subdirectory(arduino)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(test/fake)
add_subdirectory(arduino/src)
add_subdirectory(test)





