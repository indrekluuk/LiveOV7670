//
// Created by indrek on 30.10.2016.
//


#include "gtest/gtest.h"
#include "LiveOV7670Library/CameraOV7670Registers.h"



class TestCameraOV7670Registers : public ::testing::Test {
protected:
    virtual void SetUp() {
      // re-init Wire to make sure it is clean for each test
      Wire = FakeWire();
    }

    std::vector<uint8_t> registerDataToVector(const RegisterData * pRegs) {
      std::vector<uint8_t> regVector;
      for (; pRegs->addr != 0xFF; pRegs++) {
        regVector.push_back(pRegs->addr);
        regVector.push_back(pRegs->val);
      }
      return regVector;
    }
};






TEST_F(TestCameraOV7670Registers, testWriteRegisters) {
  CameraOV7670Registers registers(0);
  registers.setRegisters(CameraOV7670Registers::regsDefault);
  ASSERT_EQ(registerDataToVector(CameraOV7670Registers::regsDefault), Wire.getTransmittedBytes());
}


TEST_F(TestCameraOV7670Registers, testSetRegisterBitsOR) {
  CameraOV7670Registers registers(0);
  Wire.addBytesToRead(0b01101001);
  registers.setRegisterBitsOR(0, 0b00001111);
  ASSERT_EQ(3, Wire.getTransmittedBytes().size());
  ASSERT_EQ(0b01101111, Wire.getTransmittedBytes()[2]);
}


TEST_F(TestCameraOV7670Registers, testSetRegisterBitsAND) {
  CameraOV7670Registers registers(0);
  Wire.addBytesToRead(0b01101001);
  registers.setRegisterBitsAND(0, 0b00001111);
  ASSERT_EQ(3, Wire.getTransmittedBytes().size());
  ASSERT_EQ(0b00001001, Wire.getTransmittedBytes()[2]);
}


