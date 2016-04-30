
#include "CameraOV7670.h"



CameraOV7670::CameraOV7670(PixelFormat format, uint8_t internalClockPreScaler) :
    pixelFormat(format),
    internalClockPreScaler(internalClockPreScaler) {
}



void CameraOV7670::init() {
  Wire.begin();
  initClock();
  setUpCamera();
}


void CameraOV7670::initClock() {
  // pin 3 to 8Mhz (camera clock)
  pinMode(3, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS20);
  OCR2A = 1;
  OCR2B = 0;
}


void CameraOV7670::setUpCamera() {
  resetSettings();

  setRegisters(regsDefault);
  switch (pixelFormat) {
    default:
    case PIXEL_RGB565:
      setRegisters(regsRGB565);
      break;
    case PIXEL_YUV422:
      setRegisters(regsYUV422);
      break;
  }

  setRegisters(regsQQVGA);

  setRegisters(regsClock);
  if (internalClockPreScaler > 0){
    addBitsToRegister(REG_CLKRC, internalClockPreScaler);
  }

}




void CameraOV7670::resetSettings() {
  setRegister(REG_COM7, COM7_RESET);
  delay(500);
}


void CameraOV7670::setRegisters(const RegisterData *programMemPointer) {
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

void CameraOV7670::setRegister(uint8_t addr, uint8_t val) {
  Wire.beginTransmission(i2cAddress);
  Wire.write(addr);
  Wire.write(val);
  Wire.endTransmission();
}



uint8_t CameraOV7670::readRegister(uint8_t addr) {
  Wire.beginTransmission(i2cAddress);
  Wire.write(addr);
  Wire.endTransmission();

  Wire.requestFrom(i2cAddress, 1);
  return Wire.read();
}


void CameraOV7670::addBitsToRegister(uint8_t addr, uint8_t bits) {
  uint8_t val = readRegister(addr);
  setRegister(addr, val | bits);
}






