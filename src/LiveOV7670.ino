
#include "screen/Adafruit_ST7735_mod.h"
#include "camera/OV7670Initializer.h"


OV7670Initializer cameraInitializer;

int TFT_RST = 10;
int TFT_CS  = 9;
int TFT_DC  = 8;
// TFT_SPI_clock = 13 and TFT_SPI_data = 11
Adafruit_ST7735_mod tft = Adafruit_ST7735_mod(TFT_CS, TFT_DC, TFT_RST);



/*
B (digital pin 8 to 13)
C (analog input pins)
D (digital pins 0 to 7)
*/
#define VSYNC_PORTD 0b00000100 // PIN 2
//#define HREF_PORTC 0b10000000 // PIN A7 - doesn't work!
#define PCLOCK_PORTB 0b00010000 // PIN 12
#define LOW_4_BITS_PORTC 0b00001111 // PIN A0..A3
#define HIGH_4_BITS_PORTD 0b11110000 // PIN 4..7





void setUpCamera() {

  // pin 3 to 8Mhz (camera clock)
  pinMode(3, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS20);
  OCR2A = 1;
  OCR2B = 0;

  cameraInitializer.init();
}



void setUpScreen() {
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.fillScreen(ST7735_BLACK);
}



void sleep(uint64_t milliseconds) {
  for (uint64_t d = 0; d<196*milliseconds; d++) {
    asm volatile("nop");
  }
}




void setup() {
  //Serial.begin(9600);
  setUpCamera();
  setUpScreen();
  noInterrupts();
}





void loop() {
  processFrame();
}



int screen_w = ST7735_TFTWIDTH;
int screen_h = ST7735_TFTHEIGHT_18;
int cameraPixelColCount = 160;
int cameraPixelRowCount = 120;
int scanLine;



inline void waitForVsync(void) __attribute__((always_inline));
inline void waitForRisingPixelClock(void) __attribute__((always_inline));
inline uint8_t getPixelByte(void) __attribute__((always_inline));
inline void nextScreenLineStart(void) __attribute__((always_inline));
inline void screenLineEnd(void) __attribute__((always_inline));
inline void sendPixelByte(uint8_t byte) __attribute__((always_inline));
inline void waitUntilPreviousPixelSent(void) __attribute__((always_inline));



void processFrame() {
  waitForVsync();


  int pixelColIndex = 0;
  int pixelRowIndex = 0;



  scanLine = screen_w;
  nextScreenLineStart();

  uint8_t pixel_high = 0;
  uint8_t pixel_low = 0;


  while (true) {
    bool isPixelVisible = (pixelColIndex < screen_h);

    waitForRisingPixelClock();
    pixel_low = getPixelByte();

    if (isPixelVisible && pixelColIndex > 0) {
      sendPixelByte(pixel_low);
    }

    waitForRisingPixelClock();
    pixel_high = getPixelByte();

    if (isPixelVisible) {
      sendPixelByte(pixel_high);
    }
    
    pixelColIndex++;
    if (pixelColIndex == cameraPixelColCount) {

      waitUntilPreviousPixelSent();
      sendPixelByte(pixel_low);

      pixelColIndex = 0;
      pixelRowIndex++;

      screenLineEnd();
      nextScreenLineStart();

      if (pixelRowIndex == cameraPixelRowCount) {
        break;
      }
    }
  }

  screenLineEnd();
}







void waitForVsync()   {
  while(!(PIND & VSYNC_PORTD));
}


void waitForRisingPixelClock() {
  while(PINB & PCLOCK_PORTB);
  while(!(PINB & PCLOCK_PORTB));
}


uint8_t getPixelByte() {
  return (PINC & LOW_4_BITS_PORTC) | (PIND & HIGH_4_BITS_PORTD);
}


void nextScreenLineStart()   {
  if (scanLine > 0) scanLine--;
  tft.startAddrWindow(scanLine, 0, scanLine, screen_h-1);
}


void screenLineEnd()   {
  tft.endAddrWindow();
}


void sendPixelByte(uint8_t byte) {
  SPDR = byte;
}

void waitUntilPreviousPixelSent() {
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






