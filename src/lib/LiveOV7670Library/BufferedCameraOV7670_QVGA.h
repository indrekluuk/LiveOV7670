//
// Created by indrek on 30.04.2016.
//

#ifndef _BUFFEREDCAMERAOV7670_QVGA_H
#define _BUFFEREDCAMERAOV7670_QVGA_H

#include "BufferedCameraOV7670.h"


// 320 x 240 @ 2.5Hz or less
class BufferedCameraOV7670_QVGA : public BufferedCameraOV7670<uint16_t, 640, uint16_t, 320, uint8_t, 240> {

public:
  enum FramesPerSecond {
    FPS_2p5_Hz,
    FPS_2_Hz,
    FPS_1p66_Hz,
    FPS_1p43_Hz,
    FPS_1p25_Hz
  };


private:
  FramesPerSecond framesPerSecond;

public:
  BufferedCameraOV7670_QVGA(PixelFormat format, FramesPerSecond fps) :
      BufferedCameraOV7670(Resolution::RESOLUTION_QVGA_320x240, format, getPreScalerForFps(fps)),
      framesPerSecond(fps)
  {};


  void ignoreVerticalPadding() override;
  void readLine() override;



private:
  static uint8_t getPreScalerForFps(FramesPerSecond fps) {
    switch (fps) {
      default:
      case FPS_2p5_Hz:
        return 3;
      case FPS_2_Hz:
        return 4;
      case FPS_1p66_Hz:
        return 5;
      case FPS_1p43_Hz:
        return 6;
      case FPS_1p25_Hz:
        return 7;
    }
  }

};




void BufferedCameraOV7670_QVGA::ignoreVerticalPadding() {
  if (framesPerSecond == FPS_2p5_Hz) {
    for (uint8_t i = 0; i < verticalPadding; i++) {
      readLine();
    }
  } else {
    CameraOV7670::ignoreVerticalPadding();
  }
}


void BufferedCameraOV7670_QVGA::readLine() {

  // reading loop is too tight for 2.5Hz to wait for raising clock edge
  if (framesPerSecond == FPS_2p5_Hz) {
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






#endif //_BUFFEREDCAMERAOV7670_QVGA_H
