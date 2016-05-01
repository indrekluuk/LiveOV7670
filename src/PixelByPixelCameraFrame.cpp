//
// Created by indrek on 1.05.2016.
//

// set BUFFERED_CAMERA to 0 in setup.h to activate
#include "setup.h"
#if BUFFERED_CAMERA == 0


#include "Arduino.h"
#include "screen/Adafruit_ST7735_mod.h"
#include "camera/base/CameraOV7670.h"


// scaler values for specific refresh rates
static const uint8_t FPS_1_Hz = 9;
static const uint8_t FPS_0p5_Hz = 19;
static const uint8_t FPS_0p33_Hz = 29;


static const uint16_t lineLength = 640;
static const uint16_t lineCount = 480;



// Since the 1.8" TFT screen is only 160x128 only top right corner of the VGA picture is visible.
CameraOV7670 camera(CameraOV7670::RESOLUTION_VGA_640x480, CameraOV7670::PIXEL_RGB565, FPS_1_Hz);



int TFT_RST = 10;
int TFT_CS  = 9;
int TFT_DC  = 8;
// TFT_SPI_clock = 13 and TFT_SPI_data = 11
Adafruit_ST7735_mod tft = Adafruit_ST7735_mod(TFT_CS, TFT_DC, TFT_RST);




// this is called in Arduino setup() function
void initializeScreenAndCamera() {
  camera.init();
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  noInterrupts();
}




inline void screenLineStart(void) __attribute__((always_inline));
inline void screenLineEnd(void) __attribute__((always_inline));
inline void sendPixelByte(uint8_t byte) __attribute__((always_inline));
inline void pixelSendingDelay() __attribute__((always_inline));



// Normally it is a portrait screen. Use it as landscape
uint8_t screen_w = ST7735_TFTHEIGHT_18;
uint8_t screen_h = ST7735_TFTWIDTH;
uint8_t screenLineIndex;





// this is called in Arduino loop() function
void processFrame() {
  screenLineIndex = screen_h;

  camera.waitForVsync();

  uint8_t lowByte = 0;
  uint8_t bufferedLowByte = 0;

  for (uint16_t y = 0; y < lineCount; y++) {

    screenLineStart();

    // For full VGA resolution byte order from camera is low1, high1, low2, high2
    // We have to swap byte order for the screen.

    camera.waitForPixelClockRisingEdge();
    bufferedLowByte = camera.readPixelByte();

    for (uint16_t x = 0; x < lineLength-1; x++) {

      camera.waitForPixelClockRisingEdge();
      sendPixelByte(camera.readPixelByte()); // send pixel high byte

      lowByte = bufferedLowByte;
      camera.waitForPixelClockRisingEdge();
      bufferedLowByte = camera.readPixelByte();
      sendPixelByte(lowByte); // send pixel low byte
    }

    // send last pixel
    camera.waitForPixelClockRisingEdge();
    sendPixelByte(camera.readPixelByte()); // send pixel high byte
    pixelSendingDelay(); // prevent sending collision
    sendPixelByte(bufferedLowByte); // send pixel low byte
    pixelSendingDelay();


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

  /*
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  */
}



void pixelSendingDelay() {
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");

}




#endif




