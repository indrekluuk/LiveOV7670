cmake_minimum_required(VERSION 3.2)
project(LiveOV7670_src)



add_library(LiveOV7670_camera
        camera/base/CameraOV7670.cpp
        camera/base/CameraOV7670Registers.cpp
        camera/base/CameraOV7670RegistersDefault.cpp
        camera/base/CameraOV7670RegistersQQVGA.cpp
        camera/base/CameraOV7670RegistersQVGA.cpp
        camera/base/CameraOV7670RegistersVGA.cpp
        camera/base/CameraOV7670RegistersRGB565.cpp
        camera/base/CameraOV7670RegistersBayerRGB.cpp
        camera/base/CameraOV7670RegistersYUV422.cpp
        )


# Adding files to executable helps CLion to recognize source files as part of the project
add_executable(LiveOV7670_main
        LiveOV7670.ino
        screen/Adafruit_ST7735_mod.cpp
        ExampleTftBufferedCameraFrame.cpp
        ExampleTftPixelByPixelCameraFrame.cpp
        ExampleUart.cpp
        )

