//
// Created by indrek on 17.04.2016.
//

#include "CameraOV7670.h"


const PROGMEM RegisterData CameraOV7670::regsClock [] = {
    {REG_COM10, COM10_PCLK_HB}, // disable pixel clock during blank lines

    // internal clock prescaler
    {REG_CLKRC,
        0x80  // enable double clock option (disabled => input / 2)
        | 0x0 // f = input / (val + 1)
    },

    {REG_COM14,
        COM14_DCWEN // enable pixel clock divider
        | 0x02 // 0..4 => 1/2/4/8/16
    },

    {0xff, 0xff}	/* END MARKER */
};

