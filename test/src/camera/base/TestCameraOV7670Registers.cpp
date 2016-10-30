//
// Created by indrek on 30.10.2016.
//


#include "gtest/gtest.h"
#include "camera/base/CameraOV7670Registers.h"



std::vector<uint8_t> registerDataToVector(const RegisterData * pRegs) {
  std::vector<uint8_t> regVector;
  for (; pRegs->addr != 0xFF; pRegs++) {
    regVector.push_back(pRegs->addr);
    regVector.push_back(pRegs->val);
  }
  return regVector;
}



TEST(TestCameraOV7670Registers, testWriteRegisters) {
  CameraOV7670Registers registers(0);
  registers.setRegisters(CameraOV7670Registers::regsDefault);
  ASSERT_EQ(registerDataToVector(CameraOV7670Registers::regsDefault), Wire.getTransmittedBytes());
}




