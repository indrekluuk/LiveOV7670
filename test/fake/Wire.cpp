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

int FakeWire::endTransmission() {
  return 0;
}

void FakeWire::requestFrom(int, int) {

}

uint8_t FakeWire::read() {
  if (bytesToRead.empty()) {
    return 0;
  } else {
    uint8_t byte = bytesToRead.front();
    bytesToRead.pop_front();
    return byte;
  }
}


const std::vector<uint8_t> & FakeWire::getTransmittedBytes() {
  return transmittedBytes;
}


void FakeWire::addBytesToRead(uint8_t byte) {
  bytesToRead.push_back(byte);
}


