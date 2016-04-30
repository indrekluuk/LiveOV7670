
#include "screen/Adafruit_ST7735_mod.h"
#include "camera/CameraOV7670_QQVGA_10hz.h"
//#include "camera/CameraOV7670_QQVGA.h"



CameraOV7670_QQVGA_10hz cameraOV7670(CameraOV7670::PIXEL_RGB565);
//CameraOV7670_QQVGA cameraOV7670(CameraOV7670::PIXEL_RGB565, CameraOV7670::FPS_5_Hz);

int TFT_RST = 10;
int TFT_CS  = 9;
int TFT_DC  = 8;
// TFT_SPI_clock = 13 and TFT_SPI_data = 11
Adafruit_ST7735_mod tft = Adafruit_ST7735_mod(TFT_CS, TFT_DC, TFT_RST);




void setup() {
  //Serial.begin(9600);
  cameraOV7670.init();
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  noInterrupts();
}




inline void sendLineToDisplay() __attribute__((always_inline));
inline void screenLineStart(void) __attribute__((always_inline));
inline void screenLineEnd(void) __attribute__((always_inline));
inline void sendPixelByte(uint8_t byte) __attribute__((always_inline));




// Normally it is a portrait screen. Use it as landscape
uint8_t screen_w = ST7735_TFTHEIGHT_18;
uint8_t screen_h = ST7735_TFTWIDTH;
uint8_t screenLineIndex;


void loop() {
  screenLineIndex = screen_h;

  cameraOV7670.waitForVsync();

  for (uint8_t i = 0; i < cameraOV7670.getLineCount(); i++) {
    cameraOV7670.readLine();
    sendLineToDisplay();
  }
}





void sendLineToDisplay() {
  screenLineStart();
  for (uint16_t i=0; i<cameraOV7670.getPixelBufferLength(); i++) {
    sendPixelByte(cameraOV7670.getPixelByte(i));
  }
  screenLineEnd();
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

