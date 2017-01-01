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


CameraOV7670::PixelFormat pixelFormat = CameraOV7670::PIXEL_RGB565;
CameraOV7670 LiveOV7670(CameraOV7670::RESOLUTION_QQVGA_160x120, pixelFormat, 4);




// this is called in Arduino setup() function
void initializeScreenAndCamera() {
  Serial.begin(1000000); // 2000000 is unreliable
  LiveOV7670.init();
  noInterrupts();
}




inline void endOfFrame(void) __attribute__((always_inline));
inline void endOfLine(void) __attribute__((always_inline));
inline void sendPixelByte(uint8_t byte) __attribute__((always_inline));
inline void pixelSendingDelay() __attribute__((always_inline));


uint8_t lineBuffer [lineLength*2 + 1];
uint16_t lineBufferIndex = 0;

// this is called in Arduino loop() function
void processFrame() {
  LiveOV7670.waitForVsync();

  for (uint16_t y = 0; y < lineCount; y++) {
    lineBufferIndex = 0;
    lineBuffer[0] = 0; // first byte from Camera is half a pixel

    for (uint16_t x = 1; x < lineLength*2+1; x+=5) {
      sendPixelByte(lineBuffer[lineBufferIndex]);
      lineBufferIndex++;

      LiveOV7670.waitForPixelClockRisingEdge();
      lineBuffer[x] = LiveOV7670.readPixelByte();

      LiveOV7670.waitForPixelClockRisingEdge();
      lineBuffer[x+1] = LiveOV7670.readPixelByte();

      LiveOV7670.waitForPixelClockRisingEdge();
      lineBuffer[x+2] = LiveOV7670.readPixelByte();

      LiveOV7670.waitForPixelClockRisingEdge();
      lineBuffer[x+3] = LiveOV7670.readPixelByte();

      LiveOV7670.waitForPixelClockRisingEdge();
      lineBuffer[x+4] = LiveOV7670.readPixelByte();
    }

    while (lineBufferIndex < lineLength * 2) {
      sendPixelByte(lineBuffer[lineBufferIndex]);
      lineBufferIndex++;
      pixelSendingDelay();
    }
    sendPixelByte(0);
    pixelSendingDelay();

    endOfLine();
  }

  endOfFrame();
}








void endOfFrame()   {
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

  UDR0 = 0xFE;
  pixelSendingDelay();
  UDR0 = 0xFE;
  pixelSendingDelay();
  UDR0 = 0x01;
  pixelSendingDelay();
  UDR0 = 0x01;
  pixelSendingDelay();
  UDR0 = 0x01;
  pixelSendingDelay();
}



void endOfLine()   {
  //frame start
  UDR0 = 0x01;
  pixelSendingDelay();
  UDR0 = 0x01;
  pixelSendingDelay();
  UDR0 = 0xFE;
  pixelSendingDelay();
  UDR0 = 0xFE;
  pixelSendingDelay();
  UDR0 = 0xFE;
  pixelSendingDelay();
}


void sendPixelByte(uint8_t byte) {
  UDR0 = byte;
}



void pixelSendingDelay() {
  while(!( UCSR0A & (1<<UDRE0)));//wait for byte to transmit
}




#endif




