//
// Created by indrek on 17.04.2016.
//

#include "CameraOV7670Registers.h"


/*
 * https://github.com/ComputerNerd/ov7670-no-ram-arduino-uno/blob/master/ov7670.c
 */
const PROGMEM RegisterData CameraOV7670Registers::regsRGB565 [] = {
    {REG_COM7, COM7_RGB}, /* Selects RGB mode */
    {REG_RGB444, 0},	  /* No RGB444 please */
    {REG_COM1, 0x0},
    {REG_COM15, COM15_RGB565|COM15_R00FF},
    {REG_COM9, 0x6A},	 /* 128x gain ceiling; 0x8 is reserved bit */
    {0x4f, 0xb3},		 /* "matrix coefficient 1" */
    {0x50, 0xb3},		 /* "matrix coefficient 2" */
    {0x51, 0},		 /* vb */
    {0x52, 0x3d},		 /* "matrix coefficient 4" */
    {0x53, 0xa7},		 /* "matrix coefficient 5" */
    {0x54, 0xe4},		 /* "matrix coefficient 6" */
    {REG_COM13, /*COM13_GAMMA|*/COM13_UVSAT},
    {0xff, 0xff}	/* END MARKER */
};

