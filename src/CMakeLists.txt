cmake_minimum_required(VERSION 3.2)
project(LiveOV7670_src)



add_library(LiveOV7670_camera
        camera/base/CameraOV7670.cpp
        camera/base/CameraOV7670RegsDefault.cpp
        camera/base/CameraOV7670RegsQQVGA.cpp
        camera/base/CameraOV7670RegsQVGA.cpp
        camera/base/CameraOV7670RegsVGA.cpp
        camera/base/CameraOV7670RegsRGB565.cpp
        camera/base/CameraOV7670RegsBayerRGB.cpp
        camera/base/CameraOV7670RegsYUV422.cpp
        )


# Adding files to executable helps CLion to recognize source files as part of the project
add_executable(LiveOV7670_main
        LiveOV7670.ino
        screen/Adafruit_ST7735_mod.cpp
        ExampleTftBufferedCameraFrame.cpp
        ExampleTftPixelByPixelCameraFrame.cpp
        ExampleUart.cpp
        )

