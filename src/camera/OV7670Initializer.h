

// https://github.com/ComputerNerd/ov7670-no-ram-arduino-uno

#ifndef _OV7670_INITIALIZER_h_
#define _OV7670_INITIALIZER_h_

#include "Arduino.h"
#include "Wire.h"
#include "OV7670RegisterDefinitions.h"


class OV7670Initializer {


private:
  int i2cAddress = 0x21;
  static const RegisterData regsDefault[];
  static const RegisterData regsRGB565[];
  static const RegisterData regsQQVGA[];
  static const RegisterData regsClock[];


public:

  OV7670Initializer();
  void init();



private:
  void resetSettings();
  void setUpCamera();
  void setRegisters(const RegisterData *registerData);
  void setRegister(uint8_t addr, uint8_t val);
  uint8_t readRegister(uint8_t addr);
  void addBitsToRegister(uint8_t addr, uint8_t bits);

};


#endif // _OV7670_INITIALIZER_h_

