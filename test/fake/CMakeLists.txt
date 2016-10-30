cmake_minimum_required(VERSION 3.2)
project(LiveOV7670_fakeArduino)



add_library(fakeArduino
        Arduino.cpp
        Wire.cpp
        OV7670Simulator.cpp)

