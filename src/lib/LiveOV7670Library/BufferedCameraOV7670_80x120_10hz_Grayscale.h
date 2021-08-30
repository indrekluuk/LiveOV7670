//
// Created by indrek on 30.04.2016.
//

#ifndef _BUFFEREDCAMERAOV7670_80x120_10HZ_GRAYSCALE_H
#define _BUFFEREDCAMERAOV7670_80x120_10HZ_GRAYSCALE_H

#include "BufferedCameraOV7670.h"


// 160 x 120 @ 10Hz
class BufferedCameraOV7670_80x120_10hz_Grayscale : public BufferedCameraOV7670<uint8_t, 80, uint8_t, 80, uint8_t, 120> {


public:
    BufferedCameraOV7670_80x120_10hz_Grayscale() : BufferedCameraOV7670(Resolution::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_YUV422, 0) {};

    void readLine() override;

};




void BufferedCameraOV7670_80x120_10hz_Grayscale::readLine() {
  pixelBuffer.writeBufferPadding = 0;
  waitForPixelClockLow();

  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");


  for (uint8_t i = 0; i<80; i++) {

    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");

    readPixelByte(pixelBuffer.readBuffer[i]);
  }
}


#endif //_BUFFEREDCAMERAOV7670_80x120_10HZ_GRAYSCALE_H
