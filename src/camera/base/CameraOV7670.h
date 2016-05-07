

#ifndef _CAMERA_OV7670_h_
#define _CAMERA_OV7670_h_

#include "Arduino.h"
#include "Wire.h"
#include "CameraOV7670RegisterDefinitions.h"



/*
B (digital pin 8 to 13)
C (analog input pins)
D (digital pins 0 to 7)
*/

#ifndef OV7670_VSYNC_PIN_REG
// PIN 2
#define OV7670_VSYNC_PIN_REG PIND
#define OV7670_VSYNC_MASK 0b00000100
#endif

#ifndef OV7670_PCLOCK_PIN_REG
// PIN 12
#define OV7670_PCLOCK_PIN_REG PINB
#define OV7670_PCLOCK_MASK 0b00010000
#endif

#ifndef OV7670_LOW_4_BITS_PIN_REG
// PIN A0..A3
#define OV7670_LOW_4_BITS_PIN_REG PINC
#define OV7670_LOW_4_BITS_MASK 0b00001111
#endif

#ifndef OV7670_HIGH_4_BITS_PIN_REG
// PIN 4..7
#define OV7670_HIGH_4_BITS_PIN_REG PIND
#define OV7670_HIGH_4_BITS_MASK 0b11110000
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
  static const int i2cAddress = 0x21;
  static const RegisterData regsDefault[];
  static const RegisterData regsRGB565[];
  static const RegisterData regsBayerRGB[];
  static const RegisterData regsYUV422[];
  static const RegisterData regsQQVGA[];
  static const RegisterData regsQVGA[];
  static const RegisterData regsVGA[];

  Resolution resolution;
  PixelFormat pixelFormat;
  uint8_t internalClockPreScaler;


public:

  CameraOV7670(Resolution resolution, PixelFormat format, uint8_t internalClockPreScaler) :
      resolution(resolution),
      pixelFormat(format),
      internalClockPreScaler(internalClockPreScaler) {};

  void init();
  inline void waitForVsync(void) __attribute__((always_inline));
  inline void waitForPixelClockRisingEdge(void) __attribute__((always_inline));
  inline void waitForPixelClockLow(void) __attribute__((always_inline));
  inline void waitForPixelClockHigh(void) __attribute__((always_inline));
  inline uint8_t readPixelByte(void) __attribute__((always_inline));


private:
  void initClock();
  void resetSettings();
  void setUpCamera();
  void setRegisters(const RegisterData *registerData);
  void setRegister(uint8_t addr, uint8_t val);
  uint8_t readRegister(uint8_t addr);
  void addBitsToRegister(uint8_t addr, uint8_t bits);

};



void CameraOV7670::waitForVsync() {
  while(!(OV7670_VSYNC_PIN_REG & OV7670_VSYNC_MASK));
}

void CameraOV7670::waitForPixelClockRisingEdge() {
  waitForPixelClockLow();
  waitForPixelClockHigh();
}

void CameraOV7670::waitForPixelClockLow() {
  while(OV7670_PCLOCK_PIN_REG & OV7670_PCLOCK_MASK);
}

void CameraOV7670::waitForPixelClockHigh() {
  while(!(OV7670_PCLOCK_PIN_REG & OV7670_PCLOCK_MASK));
}

uint8_t CameraOV7670::readPixelByte() {
  return (OV7670_LOW_4_BITS_PIN_REG & OV7670_LOW_4_BITS_MASK)
         | (OV7670_HIGH_4_BITS_PIN_REG & OV7670_HIGH_4_BITS_MASK);
}


#endif // _CAMERA_OV7670_h_

