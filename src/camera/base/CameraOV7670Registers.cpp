//
// Created by indrek on 30.10.2016.
//

#include "CameraOV7670Registers.h"





void CameraOV7670Registers::resetSettings() {
  setRegister(REG_COM7, COM7_RESET);
  delay(500);
}




void CameraOV7670Registers::setRegisters(const RegisterData *programMemPointer) {
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



void CameraOV7670Registers::setRegister(uint8_t addr, uint8_t val) {
  Wire.beginTransmission(i2cAddress);
  Wire.write(addr);
  Wire.write(val);
  Wire.endTransmission();
}



uint8_t CameraOV7670Registers::readRegister(uint8_t addr) {
  Wire.beginTransmission(i2cAddress);
  Wire.write(addr);
  Wire.endTransmission();

  Wire.requestFrom(i2cAddress, (uint8_t)1);
  return Wire.read();
}


void CameraOV7670Registers::addBitsToRegister(uint8_t addr, uint8_t bits) {
  uint8_t val = readRegister(addr);
  setRegister(addr, val | bits);
}



void CameraOV7670Registers::setDisablePixelClockDuringBlankLines() {
  setRegister(REG_COM10, COM10_PCLK_HB);
}



void CameraOV7670Registers::setInternalClockPreScaler(int preScaler) {
  setRegister(REG_CLKRC, 0x80 | preScaler); // f = input / (val + 1)
}





