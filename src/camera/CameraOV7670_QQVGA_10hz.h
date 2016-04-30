//
// Created by indrek on 30.04.2016.
//

#ifndef _CAMERAOV7670_QQVGA_10HZ_H
#define _CAMERAOV7670_QQVGA_10HZ_H

#include "base/BufferedCameraOV7670.h"


// 160 x 120 @ 10Hz
class CameraOV7670_QQVGA_10hz : public BufferedCameraOV7670<160, 120> {


public:
  CameraOV7670_QQVGA_10hz(PixelFormat format) : BufferedCameraOV7670(format, 0) {};

  inline void readLine() override __attribute__((always_inline));


private:
  inline void readPixels_unrolled_x160(uint16_t byteIndex) __attribute__((always_inline));
  inline void readPixels_unrolled_x10(uint16_t byteIndex) __attribute__((always_inline));
  inline void readPixel_unrolled(uint16_t byteIndex) __attribute__((always_inline));

};




void CameraOV7670_QQVGA_10hz::readLine() {
  pixelBuffer.writeBufferPadding = 0;
  waitForPixelClockLow();
  readPixels_unrolled_x160(0);
}


#define CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP 20
void CameraOV7670_QQVGA_10hz::readPixels_unrolled_x160(uint16_t byteIndex) {
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 0);
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 1);
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 2);
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 3);
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 4);
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 5);
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 6);
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 7);
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 8);
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 9);
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 10);
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 11);
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 12);
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 13);
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 14);
  readPixels_unrolled_x10(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X160_STEP * 15);
}

#define CameraOV7670_QQVGA_10hz_READ_PIXEL_X10_STEP 2
void CameraOV7670_QQVGA_10hz::readPixels_unrolled_x10(uint16_t byteIndex) {
  readPixel_unrolled(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X10_STEP * 0);
  readPixel_unrolled(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X10_STEP * 1);
  readPixel_unrolled(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X10_STEP * 2);
  readPixel_unrolled(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X10_STEP * 3);
  readPixel_unrolled(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X10_STEP * 4);
  readPixel_unrolled(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X10_STEP * 5);
  readPixel_unrolled(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X10_STEP * 6);
  readPixel_unrolled(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X10_STEP * 7);
  readPixel_unrolled(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X10_STEP * 8);
  readPixel_unrolled(byteIndex + CameraOV7670_QQVGA_10hz_READ_PIXEL_X10_STEP * 9);
}

void CameraOV7670_QQVGA_10hz::readPixel_unrolled(uint16_t byteIndex) {
  asm volatile("nop");
  pixelBuffer.writeBuffer[byteIndex + 0] = readPixelByte();
  asm volatile("nop");
  pixelBuffer.writeBuffer[byteIndex + 1] = readPixelByte();
}




#endif //_CAMERAOV7670_QQVGA_10HZ_H
