
#include "CameraOV7670.h"


bool CameraOV7670::init() {
  registers.init();
  initIO();
  delay(10); // give camera some time to run before starting setup
  return setUpCamera();
}


void CameraOV7670::initIO() {
#ifdef OV7670_INIT_INPUTS
  OV7670_INIT_INPUTS;
#endif
  OV7670_INIT_CLOCK_OUT;
}


bool CameraOV7670::setUpCamera() {
  if (registers.resetSettings()) {
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
        verticalPadding = CameraOV7670Registers::VGA_VERTICAL_PADDING;
        break;
      case RESOLUTION_QVGA_320x240:
        registers.setRegisters(CameraOV7670Registers::regsQVGA);
        verticalPadding = CameraOV7670Registers::QVGA_VERTICAL_PADDING;
        break;
      default:
      case RESOLUTION_QQVGA_160x120:
        registers.setRegisters(CameraOV7670Registers::regsQQVGA);
        verticalPadding = CameraOV7670Registers::QQVGA_VERTICAL_PADDING;
        break;
    }

    registers.setDisablePixelClockDuringBlankLines();
    registers.setDisableHREFDuringBlankLines();
    registers.setInternalClockPreScaler(internalClockPreScaler);
    registers.setPLLMultiplier(pllMultiplier);

    return true;
  } else {
    return false;
  }
}


bool CameraOV7670::setRegister(uint8_t addr, uint8_t val) {
  registers.setRegister(addr, val);
}

uint8_t CameraOV7670::readRegister(uint8_t addr) {
  return registers.readRegister(addr);
}

void CameraOV7670::setRegisterBitsOR(uint8_t addr, uint8_t bits) {
  registers.setRegisterBitsOR(addr, bits);
}

void CameraOV7670::setRegisterBitsAND(uint8_t addr, uint8_t bits) {
  registers.setRegisterBitsAND(addr, bits);
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

void CameraOV7670::showColorBars(bool transparent) {
  registers.setShowColorBar(transparent);
}

void CameraOV7670::ignoreVerticalPadding() {
  for (uint8_t i = 0; i < verticalPadding; i++) {
    ignoreHorizontalPaddingLeft();
    for (uint16_t x = 0; x < resolution * 2; x++) {
      waitForPixelClockRisingEdge();
    }
    ignoreHorizontalPaddingRight();
  }
}


