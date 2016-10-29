//
// Created by indrek on 29.10.2016.
//

#ifndef LIVEOV7670_WIRE_H
#define LIVEOV7670_WIRE_H


#include "Arduino.h"


class FakeWire {


public:
    void begin() {}
    void beginTransmission(int) {}
    void write(int) {}
    void endTransmission() {}
    void requestFrom(int, int) {}
    uint8_t read() {return 0;}

};

extern FakeWire Wire;


#endif //LIVEOV7670_WIRE_H


