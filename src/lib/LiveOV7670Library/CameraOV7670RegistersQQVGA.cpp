//
// Created by indrek on 17.04.2016.
//
#include "CameraOV7670Registers.h"

// First few lines are garbage.
// For some reason increasing vstart will not remove the first line, and causes synchronization problems.
// It is easier read all lines from the beginning and ignore the garbage lines in the code.
const uint8_t CameraOV7670Registers::QQVGA_VERTICAL_PADDING = 2;
const uint16_t vstart = 0;
const uint16_t vstop = 120 + CameraOV7670Registers::QQVGA_VERTICAL_PADDING;

// 120 + 2 pixel (4 bytes) for padding.
// One from the beginning and three at the end.
const uint16_t hstart = 182;
const uint16_t hstop = 46;

const PROGMEM RegisterData CameraOV7670Registers::regsQQVGA [] = {
    {REG_VSTART,vstart},
    {REG_VSTOP,vstop},
    {REG_VREF,0},
    {REG_HSTART,hstart >> 3},
    {REG_HSTOP,hstop >> 3},
    {REG_HREF,0b00000000 | (hstart & 0b111) | ((hstop & 0b111) << 3)},

    {REG_COM3, COM3_DCWEN}, // enable downsamp/crop/window
    {REG_COM14, 0x1a},        // divide by 4
    {SCALING_DCWCTR, 0x22},   // downsample by 4
    {SCALING_PCLK_DIV, 0xf2}, // divide by 4

    {0xff, 0xff},	/* END MARKER */
};

