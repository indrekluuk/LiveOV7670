//
// Created by indrek on 17.04.2016.
//

#include "CameraOV7670Registers.h"


/*
 * https://github.com/ComputerNerd/ov7670-no-ram-arduino-uno/blob/master/ov7670.c
 */
const PROGMEM RegisterData CameraOV7670Registers::regsVGA [] = {





    {REG_HSTART,0x13},		// HSTART
    {REG_HSTOP,0x01},		// HSTOP
    {REG_HREF,0xF6},	// was B6
    {REG_VSTART,0x02},		// VSTART
    {REG_VSTOP,0x7a},		// VSTOP
    {REG_VREF,0x0a},	// VREF
    {0xff, 0xff},		/* END MARKER */
};

