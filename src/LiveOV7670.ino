
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
  tft.fillScreen(ST7735_RED);
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





inline void waitForVsync(void) __attribute__((always_inline));
inline void waitForRisingPixelClock(void) __attribute__((always_inline));

inline void readPixel_x160(uint16_t byteIndex) __attribute__((always_inline));
inline void readPixel_x10(uint16_t byteIndex) __attribute__((always_inline));
inline void readPixel(uint16_t byteIndex) __attribute__((always_inline));
inline uint8_t getPixelByte(void) __attribute__((always_inline));

inline void sendLineBufferToDisplay() __attribute__((always_inline));
inline void screenLineStart(void) __attribute__((always_inline));
inline void screenLineEnd(void) __attribute__((always_inline));
inline void sendPixelByte(uint8_t byte) __attribute__((always_inline));



uint8_t screen_w = ST7735_TFTWIDTH;
uint8_t screen_h = ST7735_TFTHEIGHT_18;
const uint8_t cameraPixelColCount = 160;
const uint8_t cameraPixelRowCount = 120;
uint8_t scanLine;
uint8_t lineBuffer[cameraPixelColCount * 2];



void processFrame() {
  waitForVsync();


  uint8_t pixelRowIndex = 0;

  scanLine = screen_w;


  while (pixelRowIndex < cameraPixelRowCount) {

    readPixel_x160(0);

    sendLineBufferToDisplay();
    pixelRowIndex++;
  }


}







void waitForVsync()   {
  while(!(PIND & VSYNC_PORTD));
}


void waitForRisingPixelClock() {
  while(PINB & PCLOCK_PORTB);
  while(!(PINB & PCLOCK_PORTB));
}


#define READ_PIXEL_X160_STEP 20
void readPixel_x160(uint16_t byteIndex) {
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 0);
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 1);
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 2);
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 3);
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 4);
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 5);
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 6);
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 7);
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 8);
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 9);
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 10);
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 11);
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 12);
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 13);
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 14);
  readPixel_x10(byteIndex + READ_PIXEL_X160_STEP * 15);
}

#define READ_PIXEL_X10_STEP 2
void readPixel_x10(uint16_t byteIndex) {
  readPixel(byteIndex + READ_PIXEL_X10_STEP * 0);
  readPixel(byteIndex + READ_PIXEL_X10_STEP * 1);
  readPixel(byteIndex + READ_PIXEL_X10_STEP * 2);
  readPixel(byteIndex + READ_PIXEL_X10_STEP * 3);
  readPixel(byteIndex + READ_PIXEL_X10_STEP * 4);
  readPixel(byteIndex + READ_PIXEL_X10_STEP * 5);
  readPixel(byteIndex + READ_PIXEL_X10_STEP * 6);
  readPixel(byteIndex + READ_PIXEL_X10_STEP * 7);
  readPixel(byteIndex + READ_PIXEL_X10_STEP * 8);
  readPixel(byteIndex + READ_PIXEL_X10_STEP * 9);
}

void readPixel(uint16_t byteIndex) {
  waitForRisingPixelClock();
  lineBuffer[byteIndex] = getPixelByte();
  waitForRisingPixelClock();
  lineBuffer[byteIndex+1] = getPixelByte();
}

uint8_t getPixelByte() {
  return (PINC & LOW_4_BITS_PORTC) | (PIND & HIGH_4_BITS_PORTD);
}




void sendLineBufferToDisplay() {
  screenLineStart();

  // bytes from camera are out of sync by one byte
  sendPixelByte(0);
  for (uint16_t i=0; i<(cameraPixelColCount * 2) - 1; i++) {
    sendPixelByte(lineBuffer[i]);
  }

  screenLineEnd();
}


void screenLineStart()   {
  if (scanLine > 0) scanLine--;
  tft.startAddrWindow(scanLine, 0, scanLine, screen_h-1);
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
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
}





