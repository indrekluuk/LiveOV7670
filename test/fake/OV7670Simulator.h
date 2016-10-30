//
// Created by indrek on 30.10.2016.
//

#ifndef LIVEOV7670_OV7670SIMULATOR_H
#define LIVEOV7670_OV7670SIMULATOR_H


#include "Arduino.h"
#include <functional>



#define OV7670_VSYNC ov7670Simulator.isVsync()
#define OV7670_PIXEL_CLOCK ov7670Simulator.isPixelClock()
#define OV7670_PIXEL_BYTE ov7670Simulator.getPixelByte()


class OV7670Simulator {


public:
    bool isVsync();
    bool isPixelClock();
    uint8_t getPixelByte();


    void setIsVsyncCallback(std::function<bool()> callback) {isVsyncCallback = callback;}
    void setIsPixelClockCallback(std::function<bool()> callback) {isPixelClockCallback = callback;}
    void setGetPixelCallback(std::function<uint8_t ()> callback) {getPixelCallback = callback;}


private:
    std::function<bool()> isVsyncCallback;
    std::function<bool()> isPixelClockCallback;
    std::function<uint8_t()> getPixelCallback;

};



extern OV7670Simulator ov7670Simulator;



#endif //LIVEOV7670_OV7670SIMULATOR_H
