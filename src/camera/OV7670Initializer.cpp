
#include "OV7670Initializer.h"



OV7670Initializer::OV7670Initializer() {
}


void OV7670Initializer::init() {
  Wire.begin();
  setUpCamera();
}



void OV7670Initializer::setUpCamera() {
  resetSettings();
  setRegisters(regsDefault);
  setRegisters(regsRGB565);
  setRegisters(regsQQVGA);
  setRegisters(regsClock);
}




void OV7670Initializer::resetSettings() {
  setRegister(REG_COM7, COM7_RESET);
  delay(500);
}


void OV7670Initializer::setRegisters(const RegisterData *programMemPointer) {
  while (true) {
    RegisterData regData = {
        addr: pgm_read_byte(&(programMemPointer->addr)),
        val: pgm_read_byte(&(programMemPointer->val))
    };
    if (regData.addr == 0xFF) {
      break;
    } else {
      setRegister(regData.addr, regData.val);
      programMemPointer++;
    }
  }
}

void OV7670Initializer::setRegister(uint8_t addr, uint8_t val) {
  Wire.beginTransmission(i2cAddress);
  Wire.write(addr);
  Wire.write(val);
  Wire.endTransmission();
}



uint8_t OV7670Initializer::readRegister(uint8_t addr) {
  Wire.beginTransmission(i2cAddress);
  Wire.write(addr);
  Wire.endTransmission();

  Wire.requestFrom(i2cAddress, 1);
  return Wire.read();
}


void OV7670Initializer::addBitsToRegister(uint8_t addr, uint8_t bits) {
  uint8_t val = readRegister(addr);
  setRegister(addr, val | bits);
}



