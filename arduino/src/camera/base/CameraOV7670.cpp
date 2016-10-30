
#include "CameraOV7670.h"



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
  registers.resetSettings();
  registers.setRegisters(CameraOV7670Registers::regsDefault);

  switch (pixelFormat) {
    default:
    case PIXEL_RGB565:
      registers.setRegisters(CameraOV7670Registers::regsRGB565);
      break;
    case PIXEL_BAYERRGB:
      registers.setRegisters(CameraOV7670Registers::regsBayerRGB);
      break;
    case PIXEL_YUV422:
      registers.setRegisters(CameraOV7670Registers::regsYUV422);
      break;
  }

  switch (resolution) {
    case RESOLUTION_VGA_640x480:
      registers.setRegisters(CameraOV7670Registers::regsVGA);
      break;
    case RESOLUTION_QVGA_320x240:
      registers.setRegisters(CameraOV7670Registers::regsQVGA);
      break;
    default:
    case RESOLUTION_QQVGA_160x120:
      registers.setRegisters(CameraOV7670Registers::regsQQVGA);
      break;
  }

  registers.setDisablePixelClockDuringBlankLines();
  registers.setInternalClockPreScaler(internalClockPreScaler);
}



void CameraOV7670::setManualContrastCenter(uint8_t contrastCenter) {
  registers.setManualContrastCenter(contrastCenter);
}


void CameraOV7670::setContrast(uint8_t contrast) {
  registers.setContrast(contrast);
}


void CameraOV7670::setBrightness(uint8_t birghtness) {
  registers.setBrightness(birghtness);
}


void CameraOV7670::reversePixelBits() {
  registers.reversePixelBits();
}







