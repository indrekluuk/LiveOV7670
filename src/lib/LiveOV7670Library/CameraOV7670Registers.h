//
// Created by indrek on 30.10.2016.
//

#ifndef LIVEOV7670_CAMERAOV7670REGISTERS_H
#define LIVEOV7670_CAMERAOV7670REGISTERS_H


#include "Arduino.h"
#include "Wire.h"
#include "CameraOV7670RegisterDefinitions.h"




class CameraOV7670Registers {


private:
    uint8_t i2cAddress;

public:
    static const RegisterData regsDefault[];
    static const RegisterData regsRGB565[];
    static const RegisterData regsBayerRGB[];
    static const RegisterData regsYUV422[];
    static const RegisterData regsQQVGA[];
    static const RegisterData regsQVGA[];
    static const RegisterData regsVGA[];

    static const uint8_t QQVGA_VERTICAL_PADDING;
    static const uint8_t QVGA_VERTICAL_PADDING;
    static const uint8_t VGA_VERTICAL_PADDING;

    CameraOV7670Registers(const uint8_t i2cAddress);

    void init();

    bool resetSettings();
    void setRegisters(const RegisterData *registerData);
    bool setRegister(uint8_t addr, uint8_t val);
    uint8_t readRegister(uint8_t addr);
    void setRegisterBitsOR(uint8_t addr, uint8_t bits);
    void setRegisterBitsAND(uint8_t addr, uint8_t bits);

    void setDisablePixelClockDuringBlankLines();
    void setDisableHREFDuringBlankLines();
    void setHREFReverse();
    void setInternalClockPreScaler(int preScaler);
    void setPLLMultiplier(uint8_t multiplier);
    void setManualContrastCenter(uint8_t center);
    void setContrast(uint8_t contrast);
    void setBrightness(uint8_t birghtness);
    void reversePixelBits();
    void setShowColorBar(bool transparent);

};


#endif //LIVEOV7670_CAMERAOV7670REGISTERS_H
