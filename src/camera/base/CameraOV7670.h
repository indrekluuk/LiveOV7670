

// https://github.com/ComputerNerd/ov7670-no-ram-arduino-uno

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
#define OV7670_VSYNC_PORTD 0b00000100 // PIN 2
#define OV7670_PCLOCK_PORTB 0b00010000 // PIN 12
#define OV7670_LOW_4_BITS_PORTC 0b00001111 // PIN A0..A3
#define OV7670_HIGH_4_BITS_PORTD 0b11110000 // PIN 4..7




class CameraOV7670 {

public:

  enum PixelFormat {
    PIXEL_RGB565,
    PIXEL_YUV422
  };

  enum FramesPerSecond {
    FPS_5_Hz,
    FPS_3p33_Hz,
    FPS_2p5_Hz,
    FPS_2_Hz
  };

private:
  static const int i2cAddress = 0x21;
  static const RegisterData regsDefault[];
  static const RegisterData regsRGB565[];
  static const RegisterData regsYUV422[];
  static const RegisterData regsQQVGA[];
  static const RegisterData regsClock[];

  PixelFormat pixelFormat;
  uint8_t internalClockPreScaler;


public:

  CameraOV7670(PixelFormat format, uint8_t internalClockPreScaler);
  void init();
  inline void waitForVsync(void) __attribute__((always_inline));
  inline void waitForPixelClockLow(void) __attribute__((always_inline));
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
  while(!(PIND & OV7670_VSYNC_PORTD));
}

void CameraOV7670::waitForPixelClockLow() {
  while(PINB & OV7670_PCLOCK_PORTB);
}

uint8_t CameraOV7670::readPixelByte() {
  return (PINC & OV7670_LOW_4_BITS_PORTC) | (PIND & OV7670_HIGH_4_BITS_PORTD);
}


#endif // _CAMERA_OV7670_h_

