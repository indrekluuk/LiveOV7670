//
// Created by indrek on 30.04.2016.
//

#ifndef _BUFFEREDCAMERAOV7670_QQVGA_H
#define _BUFFEREDCAMERAOV7670_QQVGA_H

#include "BufferedCameraOV7670.h"


// 160 x 120 @ 5Hz or less
class BufferedCameraOV7670_QQVGA : public BufferedCameraOV7670<uint16_t, 320, uint8_t, 160, uint8_t, 120> {

public:
  enum FramesPerSecond {
    FPS_5_Hz,
    FPS_3p33_Hz,
    FPS_2p5_Hz,
    FPS_2_Hz,
    FPS_1p66_Hz
  };


private:
  FramesPerSecond framesPerSecond;

public:
  BufferedCameraOV7670_QQVGA(PixelFormat format, FramesPerSecond fps) :
      BufferedCameraOV7670(Resolution::RESOLUTION_QQVGA_160x120, format, getPreScalerForFps(fps)),
      framesPerSecond(fps)
  {};

  void ignoreVerticalPadding() override;
  void readLine() override;


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
        return 4;
      case FPS_1p66_Hz:
        return 5;
    }
  }
};


void BufferedCameraOV7670_QQVGA::ignoreVerticalPadding() {
  if (framesPerSecond == FPS_5_Hz) {
    for (uint8_t i = 0; i < verticalPadding; i++) {
      readLine();
    }
  } else {
    CameraOV7670::ignoreVerticalPadding();
  }
}


void BufferedCameraOV7670_QQVGA::readLine() {

  // reading loop is too tight fro 5Hz to wait for raising clock edge
  if (framesPerSecond == FPS_5_Hz) {
    pixelBuffer.writeBufferPadding = 0;
    uint16_t bufferIndex = 0;

    waitForPixelClockLow();
    while (bufferIndex < getPixelBufferLength()) {
      readPixelByte(pixelBuffer.writeBuffer[bufferIndex++]);
      waitForPixelClockLow();
      readPixelByte(pixelBuffer.writeBuffer[bufferIndex++]);
      waitForPixelClockLow();
    }

  } else {
    BufferedCameraOV7670::readLine();
  }

}






#endif //_BUFFEREDCAMERAOV7670_QQVGA_H
