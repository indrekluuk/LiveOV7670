

#ifndef _CAMERA_OV7670_h_
#define _CAMERA_OV7670_h_

#include "Arduino.h"
#include "CameraOV7670Registers.h"


/*
B (digital pin 8 to 13)
C (analog input pins)
D (digital pins 0 to 7)
*/

#ifndef OV7670_VSYNC
#define OV7670_VSYNC (PIND & 0b00000100) // PIN 2
#endif

#ifndef OV7670_PIXEL_CLOCK
#define OV7670_PIXEL_CLOCK (PINB & 0b00010000) // PIN 12
#endif

#ifndef OV7670_PIXEL_BYTE
// (PIN 4..7) | (PIN A0..A3)
#define OV7670_PIXEL_BYTE ((PIND & 0b11110000) | (PINC & 0b00001111))
#endif




class CameraOV7670 {

public:

  enum PixelFormat {
    PIXEL_RGB565,
    PIXEL_BAYERRGB,
    PIXEL_YUV422
  };

  enum Resolution {
    RESOLUTION_VGA_640x480,
    RESOLUTION_QVGA_320x240,
    RESOLUTION_QQVGA_160x120
  };


private:
  static const uint8_t i2cAddress = 0x21;

  Resolution resolution;
  PixelFormat pixelFormat;
  uint8_t internalClockPreScaler;
  CameraOV7670Registers registers;


public:

  CameraOV7670(Resolution resolution, PixelFormat format, uint8_t internalClockPreScaler) :
      resolution(resolution),
      pixelFormat(format),
      internalClockPreScaler(internalClockPreScaler),
      registers(i2cAddress) {};

  void init();
  void setManualContrastCenter(uint8_t center);
  void setContrast(uint8_t contrast);
  void setBrightness(uint8_t birghtness);
  void reversePixelBits();

  inline void waitForVsync(void) __attribute__((always_inline));
  inline void waitForPixelClockRisingEdge(void) __attribute__((always_inline));
  inline void waitForPixelClockLow(void) __attribute__((always_inline));
  inline void waitForPixelClockHigh(void) __attribute__((always_inline));
  inline uint8_t readPixelByte(void) __attribute__((always_inline));


private:
  void initClock();
  void setUpCamera();
};



void CameraOV7670::waitForVsync() {
  while(!OV7670_VSYNC);
}

void CameraOV7670::waitForPixelClockRisingEdge() {
  waitForPixelClockLow();
  waitForPixelClockHigh();
}

void CameraOV7670::waitForPixelClockLow() {
  while(OV7670_PIXEL_CLOCK);
}

void CameraOV7670::waitForPixelClockHigh() {
  while(!OV7670_PIXEL_CLOCK);
}

uint8_t CameraOV7670::readPixelByte() {
  return OV7670_PIXEL_BYTE;
}


#endif // _CAMERA_OV7670_h_

