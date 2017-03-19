//
// Created by indrek on 17.04.2016.
//

#include "CameraOV7670Registers.h"


/*
 * https://github.com/ComputerNerd/ov7670-no-ram-arduino-uno/blob/master/ov7670.c
 */
const PROGMEM RegisterData CameraOV7670Registers::regsQQVGA [] = {
    {REG_COM3, COM3_DCWEN}, // enable downsamp/crop/window

    {REG_COM14, 0x1a},	// divide by 4
    {0x72, 0x22},		// downsample by 4
    {0x73, 0xf2},		// divide by 4
    {REG_HSTART,0x16},
    {REG_HSTOP,0x04},
    {REG_HREF,0xa4},
    {REG_VSTART,0x02},
    {REG_VSTOP,0x7a},
    {REG_VREF,0x0a},
    {0xff, 0xff},	/* END MARKER */
};

