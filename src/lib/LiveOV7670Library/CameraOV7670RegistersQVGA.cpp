//
// Created by indrek on 17.04.2016.
//

#include "CameraOV7670Registers.h"


/*
 * https://github.com/ComputerNerd/ov7670-no-ram-arduino-uno/blob/master/ov7670.c
 */
const PROGMEM RegisterData CameraOV7670Registers::regsQVGA [] = {
    {REG_COM3, COM3_DCWEN}, // enable downsamp/crop/window

    {REG_COM14, 0x19},        // divide by 2
    {SCALING_DCWCTR, 0x11},   // downsample by 2
    {SCALING_PCLK_DIV, 0xf1}, // divide by 2
    {REG_HSTART,0x16},
    {REG_HSTOP,0x04},
    {REG_HREF,0x24},
    {REG_VSTART,0x02},
    {REG_VSTOP,0x7a},
    {REG_VREF,0x0a},
    {0xff, 0xff},	/* END MARKER */
};

