//
// Created by indrek on 30.04.2016.
//

#ifndef _CAMERAOV7670_VGA_H
#define _CAMERAOV7670_VGA_H

#include "base/CameraOV7670.h"


// 640 x 480
class CameraOV7670_VGA : public CameraOV7670 {


private:
  static const uint16_t lineLength = 640;
  static const uint16_t lineCount = 480;

public:
  enum FramesPerSecond {
    FPS_5_Hz,
    FPS_3p33_Hz,
    FPS_2p5_Hz,
    FPS_2_Hz
  };


private:
  FramesPerSecond framesPerSecond;

public:
  CameraOV7670_VGA(PixelFormat format, FramesPerSecond fps) :
      CameraOV7670(Resolution::RESOLUTION_VGA_640x480, format, getPreScalerForFps(fps)),
      framesPerSecond(fps)
  {};


  inline uint16_t getLineLength() __attribute__((always_inline));
  inline uint16_t getLineCount() __attribute__((always_inline));


private:
  static uint8_t getPreScalerForFps(FramesPerSecond fps) {
    switch (fps) {
      default:
      case FPS_5_Hz:
        return 1;
      case FPS_3p33_Hz:
        return 2;
      case FPS_2p5_Hz:
        return 3;
      case FPS_2_Hz:
        return 15;
    }
  }
};


uint16_t CameraOV7670_VGA::getLineLength() {
  return lineLength;
}


uint16_t CameraOV7670_VGA::getLineCount() {
  return lineCount;
}



#endif //_CAMERAOV7670_VGA_H
