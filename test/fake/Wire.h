//
// Created by indrek on 29.10.2016.
//

#ifndef LIVEOV7670_WIRE_H
#define LIVEOV7670_WIRE_H


#include "Arduino.h"
#include <vector>
#include <deque>


class FakeWire {


private:
    std::vector<uint8_t> transmittedBytes;
    std::deque<uint8_t> bytesToRead;


public:
    void begin();
    void beginTransmission(int);
    void write(uint8_t byte);
    int endTransmission();
    void requestFrom(int, int);
    uint8_t read();

    const std::vector<uint8_t> & getTransmittedBytes();
    void addBytesToRead(uint8_t byte);


};

extern FakeWire Wire;


#endif //LIVEOV7670_WIRE_H


