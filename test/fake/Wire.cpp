//
// Created by indrek on 29.10.2016.
//

#include "Wire.h"







FakeWire Wire;



void FakeWire::begin() {

}

void FakeWire::beginTransmission(int) {

}

void FakeWire::write(uint8_t byte) {
  transmittedBytes.push_back(byte);
}

void FakeWire::endTransmission() {

}

void FakeWire::requestFrom(int, int) {

}

uint8_t FakeWire::read() {
  return 0;
}


const std::vector<uint8_t> & FakeWire::getTransmittedBytes() {
  return transmittedBytes;
}


