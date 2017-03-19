cmake_minimum_required(VERSION 3.2)
project(LiveOV7670)


# PlatformIO build targets
# Must init platformio. See: PlatformIO_CLion_init.txt
add_subdirectory(src)




# init Google Test target
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# fake arduino headers
include_directories(test/fake)
# project headers
include_directories(src/lib)
include_directories(src/LiveOV7670)
# gtest headers
include_directories(${gtest_SOURCE_DIR}/include)



add_executable(runTests
        test/fake/Arduino.cpp
        test/fake/Wire.cpp
        test/fake/OV7670Simulator.cpp

        src/lib/LiveOV7670Library/CameraOV7670.cpp
        src/lib/LiveOV7670Library/CameraOV7670Registers.cpp
        src/lib/LiveOV7670Library/CameraOV7670RegistersDefault.cpp
        src/lib/LiveOV7670Library/CameraOV7670RegistersQQVGA.cpp
        src/lib/LiveOV7670Library/CameraOV7670RegistersQVGA.cpp
        src/lib/LiveOV7670Library/CameraOV7670RegistersVGA.cpp
        src/lib/LiveOV7670Library/CameraOV7670RegistersRGB565.cpp
        src/lib/LiveOV7670Library/CameraOV7670RegistersBayerRGB.cpp
        src/lib/LiveOV7670Library/CameraOV7670RegistersYUV422.cpp

        test/src/camera/base/TestCameraOV7670.cpp
        test/src/camera/base/TestCameraOV7670Registers.cpp
        test/src/camera/buffered/TestBufferedCameraOV7670.cpp
        )

# link
add_subdirectory(test/lib/gtest-1.7.0)
target_link_libraries(runTests gtest_main)





