//
// Created by indrek on 1.05.2016.
//

// set EXAMPLE to EXAMPLE_UART in setup.h to activate
#include "setup.h"
#if EXAMPLE == 3


#include "Arduino.h"
#include "CameraOV7670.h"

static const uint8_t COMMAND_NEW_FRAME = 0x01;
static const uint8_t COMMAND_END_OF_LINE = 0x02;
static const uint8_t COMMAND_DEBUG_DATA = 0x03;

static const uint16_t COLOR_GREEN = 0x07E0;
static const uint16_t COLOR_RED = 0xF800;

static const uint16_t UART_PIXEL_FORMAT_RGB565 = 1;
static const uint16_t UART_PIXEL_FORMAT_GRAYSCALE = 2;

// select resolution and communication speed:
// 1 - 320x240 with 2M baud (may be unreliable!)
// 2 - 320x240 with 1M baud
// 3 - 160x120 with 1M baud
// 4 - 160x120 with 115200 baud
// 5 - 320x240 grayscale with 1M baud
// 6 - 160x120 grayscale with 1M baud
// 7 - 640x480 grayscale with 1M baud (very unreliable)
#define UART_MODE 2


#if UART_MODE==1
static const uint16_t lineLength = 320;
static const uint16_t lineCount = 240;
static const uint32_t baud  = 2000000; // may be unreliable
static const uint32_t uartSendWhileReadingCount  = 1;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QVGA_320x240, CameraOV7670::PIXEL_RGB565, 11);
#endif

#if UART_MODE==2
static const uint16_t lineLength = 320;
static const uint16_t lineCount = 240;
static const uint32_t baud  = 1000000;
static const uint32_t uartSendWhileReadingCount = 2;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QVGA_320x240, CameraOV7670::PIXEL_RGB565, 18);
#endif

#if UART_MODE==3
static const uint16_t lineLength = 160;
static const uint16_t lineCount = 120;
static const uint32_t baud  = 1000000;
static const uint32_t uartSendWhileReadingCount  = 4;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_RGB565, 5);
#endif

#if UART_MODE==4
static const uint16_t lineLength = 160;
static const uint16_t lineCount = 120;
static const uint32_t baud  = 115200;
static const uint32_t uartSendWhileReadingCount  = 4;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_RGB565, 35);
#endif

#if UART_MODE==5
static const uint16_t lineLength = 320;
static const uint16_t lineCount = 240;
static const uint32_t baud  = 1000000;
static const uint32_t uartSendWhileReadingCount = 4;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QVGA_320x240, CameraOV7670::PIXEL_YUV422, 10);
#endif

#if UART_MODE==6
static const uint16_t lineLength = 160;
static const uint16_t lineCount = 120;
static const uint32_t baud  = 1000000;
static const uint32_t uartSendWhileReadingCount = 0;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_YUV422, 2);
#endif

#if UART_MODE==7
static const uint16_t lineLength = 640;
static const uint16_t lineCount = 480;
static const uint32_t baud  = 1000000;
static const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_VGA_640x480, CameraOV7670::PIXEL_YUV422, 63);
#endif


void sendBlankFrame(uint16_t color);
inline void startNewFrame(uint8_t pixelFormat) __attribute__((always_inline));
inline void endOfLine(void) __attribute__((always_inline));
inline void sendNextPixelByte() __attribute__((always_inline));
inline void sendPixelByteH(uint8_t byte) __attribute__((always_inline));
inline void sendPixelByteL(uint8_t byte) __attribute__((always_inline));
inline void sendPixelByteGrayscale(uint8_t byte) __attribute__((always_inline));
inline void pixelSendingDelay() __attribute__((always_inline));

inline void debugPrint(const String debugText) __attribute__((always_inline));


// this is called in Arduino setup() function
void initializeScreenAndCamera() {

  // Enable this for WAVGAT CPUs
  // For UART communiation we want to set WAVGAT Nano to 16Mhz to match Atmel based Arduino
  //CLKPR = 0x80; // enter clock rate change mode
  //CLKPR = 1; // set prescaler to 1. WAVGAT MCU has it 3 by default.
  
  Serial.begin(baud);
  if (camera.init()) {
    sendBlankFrame(COLOR_GREEN);
    delay(1000);
  } else {
    sendBlankFrame(COLOR_RED);
    delay(3000);
  }
}




void sendBlankFrame(uint16_t color) {
  uint8_t colorH = (color >> 8) & 0xFF;
  uint8_t colorL = color & 0xFF;

  startNewFrame(UART_PIXEL_FORMAT_RGB565);
  for (uint16_t j=0; j<lineCount; j++) {
    for (uint16_t i=0; i<lineLength; i++) {
      sendPixelByteH(colorH);
      pixelSendingDelay();
      sendPixelByteL(colorL);
      pixelSendingDelay();
    }
    endOfLine();
  }
}





uint8_t lineBuffer [lineLength*2 + 1 + 5];
uint16_t lineBufferIndex = 0;
uint16_t frameCounter = 0;


// this is called in Arduino loop() function
void processFrame() {
  startNewFrame(uartPixelFormat);

  noInterrupts();
  camera.waitForVsync();

  for (uint16_t y = 0; y < lineCount; y++) {

#if UART_MODE==7
    // separate loop for full VGA
    for (uint16_t x = 0; x < lineLength; x++) {
      // ignore first pixel byte
      camera.waitForPixelClockRisingEdge();

      // second byte is grayscale byte
      camera.waitForPixelClockRisingEdge();
      uint8_t pixelByte;
      camera.readPixelByte(pixelByte);
      sendPixelByteGrayscale(pixelByte);
    }
    // delay for last byte
    pixelSendingDelay();

#else

    lineBufferIndex = 0;
    uint8_t sendWhileReadCounter = 0;

    lineBuffer[0] = 0; // first byte from Camera is half a pixel

    for (uint16_t x = 1; x < lineLength*2+1; x++) {
      // start sending first bytes while reading pixels from camera
      if (uartSendWhileReadingCount > 0) {
        if (sendWhileReadCounter) {
          sendWhileReadCounter--;
        } else {
          sendNextPixelByte();
          sendWhileReadCounter = uartSendWhileReadingCount;
        }
      }

      camera.waitForPixelClockRisingEdge();
      camera.readPixelByte(lineBuffer[x]);
    }

    // send rest of the line
    while (lineBufferIndex < lineLength * 2) {
      sendNextPixelByte();
      pixelSendingDelay();
    }
#endif

    endOfLine();
  }
  interrupts();

  frameCounter++;
  debugPrint("Frame " + String(frameCounter));
  //debugPrint("Frame " + String(frameCounter, 16)); // send number in hexadecimal
}



void startNewFrame(uint8_t pixelFormat) {
  UDR0 = 0x00;
  pixelSendingDelay();
  UDR0 = COMMAND_NEW_FRAME;
  pixelSendingDelay();

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
}



void endOfLine()   {
  UDR0 = 0x00;
  pixelSendingDelay();
  UDR0 = COMMAND_END_OF_LINE;
  pixelSendingDelay();
}

void sendNextPixelByte() {
  if (uartPixelFormat == UART_PIXEL_FORMAT_GRAYSCALE) {
    sendPixelByteGrayscale(lineBuffer[lineBufferIndex]);
    lineBufferIndex += 2;
  } else {
    uint8_t byte = lineBuffer[lineBufferIndex];
    uint8_t isLowPixelByte = lineBufferIndex & 0x01;

    if (isLowPixelByte) {
      sendPixelByteL(byte);
    } else {
      sendPixelByteH(byte);
    }
    lineBufferIndex++;
  }
}


void sendPixelByteH(uint8_t byte) {
  //              RRRRRGGG
  UDR0 = byte | 0b00001000; // make pixel color always slightly above 0 since zero is end of line marker
}

void sendPixelByteL(uint8_t byte) {
  //              GGGBBBBB
  UDR0 = byte | 0b00100001; // make pixel color always slightly above 0 since zero is end of line marker
}

void sendPixelByteGrayscale(uint8_t byte) {
  UDR0 = byte | 0b00000001; // make pixel color always slightly above 0 since zero is end of line marker
}




void debugPrint(const String debugText) {
    UDR0 = 0x00;
    pixelSendingDelay();
    UDR0 = COMMAND_DEBUG_DATA;
    pixelSendingDelay();
    UDR0 = debugText.length();
    pixelSendingDelay();
    for (uint16_t i=0; i<debugText.length(); i++) {
        UDR0 = debugText[i];
        pixelSendingDelay();
    }
}



void pixelSendingDelay() {
  while(!( UCSR0A & (1<<UDRE0)));//wait for byte to transmit
}




#endif
