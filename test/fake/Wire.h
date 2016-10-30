//
// Created by indrek on 29.10.2016.
//

#ifndef LIVEOV7670_WIRE_H
#define LIVEOV7670_WIRE_H


#include "Arduino.h"
#include <vector>


class FakeWire {


private:
    std::vector<uint8_t> transmittedBytes;

public:
    void begin();
    void beginTransmission(int);
    void write(uint8_t byte);
    void endTransmission();
    void requestFrom(int, int);
    uint8_t read();


public:
    const std::vector<uint8_t> & getTransmittedBytes();


};

extern FakeWire Wire;


#endif //LIVEOV7670_WIRE_H


