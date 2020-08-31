//
// Created by indrek on 1.05.2016.
//

// set EXAMPLE to EXAMPLE_TFT_BUFFERED in setup.h to activate
#include "setup.h"
#if EXAMPLE == 1



#include "Arduino.h"
#include "Adafruit_ST7735_mod.h"
#include <BufferedCameraOV7670_QQVGA_10hz.h>
#include <BufferedCameraOV7670_QQVGA.h>
#include <BufferedCameraOV7670_QVGA.h>
#include <BufferedCameraOV7670_QQVGA_10hz_Grayscale.h>
#include "GrayScaleTable.h"



#define GRAYSCALE_PIXELS 0

#if GRAYSCALE_PIXELS == 1
BufferedCameraOV7670_QQVGA_10hz_Grayscale camera;
#else
BufferedCameraOV7670_QQVGA_10hz camera(CameraOV7670::PIXEL_RGB565);
//BufferedCameraOV7670_QQVGA camera(CameraOV7670::PIXEL_RGB565, BufferedCameraOV7670_QQVGA::FPS_5_Hz);
//BufferedCameraOV7670_QQVGA camera(CameraOV7670::PIXEL_RGB565, BufferedCameraOV7670_QQVGA::FPS_2_Hz);
//BufferedCameraOV7670_QVGA camera(CameraOV7670::PIXEL_RGB565, BufferedCameraOV7670_QVGA::FPS_2p5_Hz);
#endif




#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
int TFT_RST = 49;
int TFT_CS  = 53;
int TFT_DC  = 48;
// TFT_SPI_clock = 52 and TFT_SPI_data = 51
#else
int TFT_RST = 10;
int TFT_CS  = 9;
int TFT_DC  = 8;
// TFT_SPI_clock = 13 and TFT_SPI_data = 11
#endif
Adafruit_ST7735_mod tft = Adafruit_ST7735_mod(TFT_CS, TFT_DC, TFT_RST);



// this is called in Arduino setup() function
void initializeScreenAndCamera() {
  bool cameraInitialized = camera.init();
  tft.initR(INITR_BLACKTAB);
  if (cameraInitialized) {
    // flash green screen if camera setup was successful
    tft.fillScreen(ST7735_GREEN);
    delay(1000);
    tft.fillScreen(ST7735_BLACK);
  } else {
    // red screen if failed to connect to camera
    tft.fillScreen(ST7735_RED);
    delay(3000);
  }
}





inline void sendLineToDisplay() __attribute__((always_inline));
inline void screenLineStart(void) __attribute__((always_inline));
inline void screenLineEnd(void) __attribute__((always_inline));
inline void sendPixelByte(uint8_t byte) __attribute__((always_inline));


// Normally it is a portrait screen. Use it as landscape
uint8_t screen_w = ST7735_TFTHEIGHT_18;
uint8_t screen_h = ST7735_TFTWIDTH;
uint8_t screenLineIndex;




// this is called in Arduino loop() function
void processFrame() {
  screenLineIndex = screen_h;

  noInterrupts();
  camera.waitForVsync();

  for (uint8_t i = 0; i < camera.getLineCount(); i++) {
    camera.readLine();
    sendLineToDisplay();
  }
  interrupts();
}






static const uint16_t byteCountForDisplay = camera.getPixelBufferLength() < screen_w*2 ?
                                            camera.getPixelBufferLength() : screen_w*2;


void sendLineToDisplay() {
  if (screenLineIndex > 0) {

    screenLineStart();
#if GRAYSCALE_PIXELS == 1
    for (uint16_t i=0; i<camera.getLineLength(); i++) {
      sendPixelByte(graysScaleTableHigh[camera.getPixelByte(i)]);
      sendPixelByte(graysScaleTableLow[camera.getPixelByte(i)]);
    }
#else
    for (uint16_t i=0; i<byteCountForDisplay; i++) {
      sendPixelByte(camera.getPixelByte(i));
    }
#endif
    screenLineEnd();
  }
}


void screenLineStart()   {
  if (screenLineIndex > 0) screenLineIndex--;
  tft.startAddrWindow(screenLineIndex, 0, screenLineIndex, screen_w-1);
}

void screenLineEnd() {
  tft.endAddrWindow();
}


void sendPixelByte(uint8_t byte) {
  SPDR = byte;

  // this must be adjusted if sending loop has more/less instructions

  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");

#if GRAYSCALE_PIXELS == 1
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
#endif

}


#endif
