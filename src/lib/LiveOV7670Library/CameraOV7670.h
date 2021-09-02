

#ifndef _CAMERA_OV7670_h_
#define _CAMERA_OV7670_h_

#include "Arduino.h"
#include "CameraOV7670Registers.h"


// Arduino Uno/Nano.
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
/*
  B (digital pin 8 to 13)
  C (analog input pins)
  D (digital pins 0 to 7)
*/

#ifndef OV7670_VSYNC
#define OV7670_VSYNC (PIND & 0b00000100) // PIN 2
#endif

#ifndef OV7670_PIXEL_CLOCK
#define OV7670_PIXEL_CLOCK_PIN 12
#define OV7670_PIXEL_CLOCK (PINB & 0b00010000) // PIN 12
#endif

#ifndef OV7670_READ_PIXEL_BYTE
// (PIN 4..7) | (PIN A0..A3)
#define OV7670_READ_PIXEL_BYTE(b) \
                    uint8_t pinc=PINC;\
                    b=((PIND & 0b11110000) | (pinc & 0b00001111))
#endif

// pin 3 to 8Mhz (LiveOV7670Library clock)
#ifndef OV7670_INIT_CLOCK_OUT
#define OV7670_INIT_CLOCK_OUT \
                    pinMode(3, OUTPUT); \
                    TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20); \
                    TCCR2B = _BV(WGM22) | _BV(CS20); \
                    OCR2A = 1; \
                    OCR2B = 0
#endif

#endif



// Arduino Mega.
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#ifndef OV7670_VSYNC
#define OV7670_VSYNC (PINE & 0b00010000) // PIN 2
#endif

#ifndef OV7670_PIXEL_CLOCK
#define OV7670_PIXEL_CLOCK_PIN 12
#define OV7670_PIXEL_CLOCK (PINB & 0b01000000) // PIN 12
#endif

#ifndef OV7670_READ_PIXEL_BYTE
// PIN 22..29
// Add "nop" so the timing would be compatible with Uno/Nano
#define OV7670_READ_PIXEL_BYTE(b) \
                    b=PINA;\
                    asm volatile("nop");\
                    asm volatile("nop");\
                    asm volatile("nop");\
                    asm volatile("nop")
#endif

// pin 9 to 8Mhz (LiveOV7670Library clock)
#ifndef OV7670_INIT_CLOCK_OUT
#define OV7670_INIT_CLOCK_OUT \
                    pinMode(9, OUTPUT); \
                    TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20); \
                    TCCR2B = _BV(WGM22) | _BV(CS20); \
                    OCR2A = 1; \
                    OCR2B = 0
#endif

#endif




// STM32
#ifdef _VARIANT_ARDUINO_STM32_

#ifndef OV7670_INIT_INPUTS
#define OV7670_INIT_INPUTS \
              const int inputPins[]={PB3,PB4,PB5,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15}; \
              for(int i=0;i<8;i++) { \
                pinMode(inputPins[i],INPUT); \
              }
#endif

// vsync - PB5
#ifndef OV7670_VSYNC
#define OV7670_VSYNC ((*GPIOB_BASE).IDR & 0x0020)
#endif

// pixel clock - PB4
#ifndef OV7670_PIXEL_CLOCK
#define OV7670_PIXEL_CLOCK ((*GPIOB_BASE).IDR & 0x0010)
#endif

// href - PB3
#ifndef OV7670_HREF
#define OV7670_HREF ((*GPIOB_BASE).IDR & 0x0008)
#endif

// pixel byte - PB8..PB15
#ifndef OV7670_READ_PIXEL_BYTE
#define OV7670_READ_PIXEL_BYTE(b) b = ((uint8_t*)(&(*GPIOB_BASE).IDR))[1]
#endif

// configure PA8 to output PLL/2 clock
#ifndef OV7670_INIT_CLOCK_OUT
#define OV7670_INIT_CLOCK_OUT \
                    gpio_set_mode(GPIOA, 8, GPIO_AF_OUTPUT_PP); \
                    *(volatile uint8_t *)(0x40021007) = 0x7
#endif

#endif



/*
#define C_PORT_OUT ((*GPIOC_BASE).ODR)
#define C14_PIN_MASK 0x4000
#define C14_ON C_PORT_OUT |= C14_PIN_MASK
#define C14_OFF C_PORT_OUT &= ~C14_PIN_MASK
*/




class CameraOV7670 {

public:

    enum PixelFormat {
        PIXEL_RGB565,
        PIXEL_BAYERRGB,
        PIXEL_YUV422
    };

    enum Resolution {
        RESOLUTION_VGA_640x480 = 640,
        RESOLUTION_QVGA_320x240 = 320,
        RESOLUTION_QQVGA_160x120 = 160
    };

    enum PLLMultiplier {
        PLL_MULTIPLIER_BYPASS = 0,
        PLL_MULTIPLIER_X4 = 1,
        PLL_MULTIPLIER_X6 = 2,
        PLL_MULTIPLIER_X8 = 3
    };


protected:
    static const uint8_t i2cAddress = 0x21;

    const Resolution resolution;
    PixelFormat pixelFormat;
    uint8_t internalClockPreScaler;
    PLLMultiplier pllMultiplier;
    CameraOV7670Registers registers;
    uint8_t verticalPadding = 0;

public:

    CameraOV7670(
        Resolution resolution,
        PixelFormat format,
        uint8_t internalClockPreScaler,
        PLLMultiplier pllMultiplier = PLL_MULTIPLIER_BYPASS
    ) :
        resolution(resolution),
        pixelFormat(format),
        internalClockPreScaler(internalClockPreScaler),
        pllMultiplier(pllMultiplier),
        registers(i2cAddress) {};

    bool init();
    bool setRegister(uint8_t addr, uint8_t val);
    uint8_t readRegister(uint8_t addr);
    void setRegisterBitsOR(uint8_t addr, uint8_t bits);
    void setRegisterBitsAND(uint8_t addr, uint8_t bits);
    void setManualContrastCenter(uint8_t center);
    void setContrast(uint8_t contrast);
    void setBrightness(uint8_t birghtness);
    void reversePixelBits();
    void showColorBars(bool transparent);

    inline void waitForVsync(void) __attribute__((always_inline));
    inline void waitForPixelClockRisingEdge(void) __attribute__((always_inline));
    inline void waitForPixelClockLow(void) __attribute__((always_inline));
    inline void waitForPixelClockHigh(void) __attribute__((always_inline));
    inline void ignoreHorizontalPaddingLeft(void) __attribute__((always_inline));
    inline void ignoreHorizontalPaddingRight(void) __attribute__((always_inline));
    inline void readPixelByte(uint8_t & byte) __attribute__((always_inline));

    virtual void ignoreVerticalPadding();

protected:
    virtual bool setUpCamera();

private:
    void initIO();

};



void CameraOV7670::waitForVsync() {
  while(!OV7670_VSYNC);
}

void CameraOV7670::waitForPixelClockRisingEdge() {
  waitForPixelClockLow();
  waitForPixelClockHigh();
}

void CameraOV7670::waitForPixelClockLow() {
  while(OV7670_PIXEL_CLOCK);
}

void CameraOV7670::waitForPixelClockHigh() {
  while(!OV7670_PIXEL_CLOCK);
}

// One byte at the beginning
void CameraOV7670::ignoreHorizontalPaddingLeft() {
  waitForPixelClockRisingEdge();
}

// Three bytes at the end
void CameraOV7670::ignoreHorizontalPaddingRight() {
  volatile uint16_t pixelTime = 0;

  waitForPixelClockRisingEdge();
  waitForPixelClockRisingEdge();

  // After the last pixel byte of an image line there is a very small pixel clock pulse.
  // To avoid accidentally counting this small pulse we measure the length of the
  // last pulse and then wait the same time to add a byte wide delay at the
  // end of the line.
  while(OV7670_PIXEL_CLOCK) pixelTime++;
  while(!OV7670_PIXEL_CLOCK) pixelTime++;
  while(pixelTime) pixelTime--;
}

void CameraOV7670::readPixelByte(uint8_t & byte) {
  OV7670_READ_PIXEL_BYTE(byte);
}


#endif // _CAMERA_OV7670_h_

