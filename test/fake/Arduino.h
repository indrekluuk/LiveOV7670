//
// Created by indrek on 29.10.2016.
//

#ifndef LIVEOV7670_ARDUINO_H
#define LIVEOV7670_ARDUINO_H


#include <stdint.h>


extern uint8_t dummy;


#define __AVR_ATmega328P__ dummy
//#define __AVR_ATmega2560__ dummy

#define PINA 0
#define PINB 0
#define PINC 0
#define PIND 0
#define PINE 0
#define INPUT 0
#define OUTPUT 1


#define PROGMEM
#define TCCR2A dummy
#define _BV(p) p
#define COM2A1 dummy
#define COM2B1 dummy
#define WGM21 dummy
#define WGM20 dummy
#define TCCR2B dummy
#define WGM22 dummy
#define CS20 dummy
#define OCR2A dummy
#define OCR2B dummy




void pinMode(int, int);
void delay(int);
uint8_t pgm_read_byte(const uint8_t * pByte);


#endif //LIVEOV7670_ARDUINO_H



