//
// Created by indrek on 30.10.2016.
//

#include "OV7670Simulator.h"




OV7670Simulator ov7670Simulator;




bool OV7670Simulator::isVsync() {
  return isVsyncCallback ? isVsyncCallback() : true;
}

bool OV7670Simulator::isPixelClock() {
  return isPixelClockCallback ? isPixelClockCallback() : true;
}

uint8_t OV7670Simulator::getPixelByte() {
  return getPixelCallback ? getPixelCallback() : (uint8_t)0;
}



