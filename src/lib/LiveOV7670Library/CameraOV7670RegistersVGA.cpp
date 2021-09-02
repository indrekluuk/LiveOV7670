//
// Created by indrek on 17.04.2016.
//
#include "CameraOV7670Registers.h"

// First few lines are garbage.
// For some reason increasing vstart will not remove the first line, and causes synchronization problems.
// It is easier read all lines from the beginning and ignore the garbage lines in the code.
const uint8_t CameraOV7670Registers::VGA_VERTICAL_PADDING = 10;
const uint16_t vstart = 0;
const uint16_t vstop = 480 + CameraOV7670Registers::VGA_VERTICAL_PADDING;

// 480 + 2 pixel (4 bytes) for padding.
// One from the beginning and three at the end.
const uint16_t hstart = 156;
const uint16_t hstop = 14;

const PROGMEM RegisterData CameraOV7670Registers::regsVGA [] = {
    {REG_VSTART, vstart >> 2},
    {REG_VSTOP, vstop >> 2},
    {REG_VREF, (vstart & 0b11) | ((vstop & 0b11) << 2)},
    {REG_HSTART, hstart >> 3},
    {REG_HSTOP, hstop >> 3},
    {REG_HREF, 0b00000000 | (hstart & 0b111) | ((hstop & 0b111) << 3)},

    {0xff, 0xff},		/* END MARKER */
};
