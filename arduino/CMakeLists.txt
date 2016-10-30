cmake_minimum_required(VERSION 3.2)
project(arduino)

include(CMakeListsPrivate.txt)

add_definitions(-DF_CPU=16000000L)
add_definitions(-DARDUINO_ARCH_AVR)
add_definitions(-DARDUINO_AVR_NANO)
add_definitions(-DARDUINO=10608)
add_definitions(-DPLATFORMIO=020806)
add_definitions(-D__AVR_ATmega328P__)

add_custom_target(
    PLATFORMIO_BUILD ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_UPLOAD ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target upload
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_CLEAN ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target clean
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_PROGRAM ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target program
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_UPLOADFS ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion run --target uploadfs
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    PLATFORMIO_UPDATE_ALL ALL
    COMMAND ${PLATFORMIO_CMD} -f -c clion update
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(arduino
    src/ExampleTftBufferedCameraFrame.cpp
    src/ExampleTftPixelByPixelCameraFrame.cpp
    src/ExampleUart.cpp
    src/camera/base/CameraOV7670.cpp
    src/camera/base/CameraOV7670Registers.cpp
    src/camera/base/CameraOV7670RegistersBayerRGB.cpp
    src/camera/base/CameraOV7670RegistersDefault.cpp
    src/camera/base/CameraOV7670RegistersQQVGA.cpp
    src/camera/base/CameraOV7670RegistersQVGA.cpp
    src/camera/base/CameraOV7670RegistersRGB565.cpp
    src/camera/base/CameraOV7670RegistersVGA.cpp
    src/camera/base/CameraOV7670RegistersYUV422.cpp
    src/screen/Adafruit_ST7735_mod.cpp
)
