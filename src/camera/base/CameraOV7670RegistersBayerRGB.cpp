//
// Created by indrek on 17.04.2016.
//

#include "CameraOV7670Registers.h"


/*
 * https://github.com/ComputerNerd/ov7670-no-ram-arduino-uno/blob/master/ov7670.c
 */
const PROGMEM RegisterData CameraOV7670Registers::regsBayerRGB [] = {
    {REG_COM7, COM7_BAYER},
    {REG_COM13, 0x08}, /* No gamma, magic rsvd bit */
    {REG_COM16, 0x3d}, /* Edge enhancement, denoise */
    {REG_REG76, 0xe1}, /* Pix correction, magic rsvd */
    {0xff, 0xff},	/* END MARKER */
};

