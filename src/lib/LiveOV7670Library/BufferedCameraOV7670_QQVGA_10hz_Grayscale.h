//
// Created by indrek on 30.04.2016.
//

#ifndef _BUFFEREDCAMERAOV7670_QQVGA_10HZ_GRAYSCALE_H
#define _BUFFEREDCAMERAOV7670_QQVGA_10HZ_GRAYSCALE_H

#include "BufferedCameraOV7670.h"


// 160 x 120 @ 10Hz
class BufferedCameraOV7670_QQVGA_10hz_Grayscale : public BufferedCameraOV7670<uint8_t, 160, uint8_t, 160, uint8_t, 120> {


public:
  BufferedCameraOV7670_QQVGA_10hz_Grayscale() : BufferedCameraOV7670(Resolution::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_YUV422, 0) {};

  void ignoreVerticalPadding() override;
  void readLine() override;

};



void BufferedCameraOV7670_QQVGA_10hz_Grayscale::ignoreVerticalPadding() {
  for (uint8_t i = 0; i < verticalPadding; i++) {
    readLine();
  }
}


void BufferedCameraOV7670_QQVGA_10hz_Grayscale::readLine() {
  pixelBuffer.writeBufferPadding = 0;

  waitForPixelClockLow();
  waitForPixelClockHigh();

  for (uint8_t i = 0; i<160; i++) {
    waitForPixelClockLow();
    readPixelByte(pixelBuffer.readBuffer[i]);
  }
}





#endif //_BUFFEREDCAMERAOV7670_QQVGA_10HZ_GRAYSCALE_H
