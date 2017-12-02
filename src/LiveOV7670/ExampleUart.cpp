//
// Created by indrek on 1.05.2016.
//

// set EXAMPLE to EXAMPLE_UART in setup.h to activate
#include "setup.h"
#if EXAMPLE == 3


#include "Arduino.h"
#include "CameraOV7670.h"


static const uint16_t lineLength = 160;
static const uint16_t lineCount = 120;
static const uint16_t COLOR_GREEN = 0x07E0;
static const uint16_t COLOR_RED = 0xF800;



CameraOV7670::PixelFormat pixelFormat = CameraOV7670::PIXEL_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QQVGA_160x120, pixelFormat, 5);



void sendBlankFrame(uint16_t color);
inline void endOfFrame(void) __attribute__((always_inline));
inline void endOfLine(void) __attribute__((always_inline));
inline void sendNextPixelByte() __attribute__((always_inline));
inline void sendPixelByteH(uint8_t byte) __attribute__((always_inline));
inline void sendPixelByteL(uint8_t byte) __attribute__((always_inline));
inline void pixelSendingDelay() __attribute__((always_inline));




// this is called in Arduino setup() function
void initializeScreenAndCamera() {
  Serial.begin(1000000); // 2000000 is unreliable
  if (camera.init()) {
    sendBlankFrame(COLOR_GREEN);
    delay(1000);
  } else {
    sendBlankFrame(COLOR_RED);
    delay(3000);
  }
  noInterrupts();
}




void sendBlankFrame(uint16_t color) {
  uint8_t colorH = (color >> 8) & 0xFF;
  uint8_t colorL = color & 0xFF;

  for (uint16_t j=0; j<lineCount; j++) {
    for (uint16_t i=0; i<lineLength; i++) {
      sendPixelByteH(colorH);
      pixelSendingDelay();
      sendPixelByteL(colorL);
      pixelSendingDelay();
    }
    endOfLine();
  }
  endOfFrame();

}





uint8_t lineBuffer [lineLength*2 + 1 + 5];
uint16_t lineBufferIndex = 0;



// this is called in Arduino loop() function
void processFrame() {
  camera.waitForVsync();

  for (uint16_t y = 0; y < lineCount; y++) {
    lineBufferIndex = 0;
    lineBuffer[0] = 0; // first byte from Camera is half a pixel

    for (uint16_t x = 1; x < lineLength*2+1; x+=5) {
      // start sending first bytes while reading pixels from camera
      sendNextPixelByte();

      // we can read 5 bytes from camera while one byte is sent over UART
      camera.waitForPixelClockRisingEdge();
      camera.readPixelByte(lineBuffer[x]);

      camera.waitForPixelClockRisingEdge();
      camera.readPixelByte(lineBuffer[x+1]);

      camera.waitForPixelClockRisingEdge();
      camera.readPixelByte(lineBuffer[x+2]);

      camera.waitForPixelClockRisingEdge();
      camera.readPixelByte(lineBuffer[x+3]);

      camera.waitForPixelClockRisingEdge();
      camera.readPixelByte(lineBuffer[x+4]);
    }

    // send rest of the line
    while (lineBufferIndex < lineLength * 2) {
      sendNextPixelByte();
      pixelSendingDelay();
    }

    endOfLine();
  }

  endOfFrame();
}



void endOfFrame() {
  // frame width
  UDR0 = (lineLength >> 8) & 0xFF;
  pixelSendingDelay();
  UDR0 = lineLength & 0xFF;
  pixelSendingDelay();

  // frame height
  UDR0 = (lineCount >> 8) & 0xFF;
  pixelSendingDelay();
  UDR0 = lineCount & 0xFF;
  pixelSendingDelay();

  // pixel format
  UDR0 = (pixelFormat);
  pixelSendingDelay();

  UDR0 = 0xFF;
  pixelSendingDelay();
  UDR0 = 0x00;
  pixelSendingDelay();
  UDR0 = 0x00;
  pixelSendingDelay();
  UDR0 = 0x00;
  pixelSendingDelay();
}



void endOfLine()   {
  UDR0 = 0x00;
  pixelSendingDelay();
}


void sendNextPixelByte() {
  uint8_t byte = lineBuffer[lineBufferIndex];
  uint8_t isLowPixelByte = lineBufferIndex & 0x01;

  // make pixel color always slightly above 0 since zero is end of line
  if (isLowPixelByte) {
    sendPixelByteL(byte);
  } else {
    sendPixelByteH(byte);
  }
  lineBufferIndex++;
}


void sendPixelByteH(uint8_t byte) {
  //              RRRRRGGG
  UDR0 = byte | 0b00001000;
}

void sendPixelByteL(uint8_t byte) {
  //              GGGBBBBB
  UDR0 = byte | 0b00100001;
}





void pixelSendingDelay() {
  while(!( UCSR0A & (1<<UDRE0)));//wait for byte to transmit
}




#endif




