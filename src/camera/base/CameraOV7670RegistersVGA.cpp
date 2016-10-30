//
// Created by indrek on 17.04.2016.
//

#include "CameraOV7670Registers.h"


/*
 * https://github.com/ComputerNerd/ov7670-no-ram-arduino-uno/blob/master/ov7670.c
 */
const PROGMEM RegisterData CameraOV7670Registers::regsVGA [] = {
    {REG_HREF,0xF6},	// was B6
    {0x17,0x13},		// HSTART
    {0x18,0x01},		// HSTOP
    {0x19,0x02},		// VSTART
    {0x1a,0x7a},		// VSTOP
    {REG_VREF,0x0a},	// VREF
    {0xff, 0xff},		/* END MARKER */
};

