//
// Created by indrek on 1.05.2016.
//

// set EXAMPLE to EXAMPLE_UART in setup.h to activate
#include "setup.h"
#if EXAMPLE == 3
#include "Arduino.h"
#include "CameraOV7670.h"


// select resolution and communication speed:
//  1 - 115200bps 160x120 rgb
//  2 - 115200bps 160x120 grayscale
//  3 - 500000bps 160x120 rgb
//  4 - 500000bps 160x120 grayscale
//  5 - 500000bps 320x240 rgb
//  6 - 500000bps 320x240 grayscale
//  7 - 1Mbps 160x120 rgb
//  8 - 1Mbps 160x120 grayscale
//  9 - 1Mbps 320x240 rgb
// 10 - 1Mbps 320x240 grayscale
// 11 - 1Mbps 640x480 grayscale
// 12 - 2Mbps 160x120 rgb
// 13 - 2Mbps 160x120 grayscale
// 14 - 2Mbps 320x240 rgb
// 15 - 2Mbps 320x240 grayscale
// 16 - 2Mbps 640x480 rgb
// 17 - 2Mbps 640x480 grayscale
#define UART_MODE 5




const uint8_t VERSION = 0x00;
const uint8_t COMMAND_NEW_FRAME = 0x01 | VERSION;
const uint8_t COMMAND_END_OF_LINE = 0x02;
const uint8_t COMMAND_DEBUG_DATA = 0x03;

// Lower three bits 0b00000111.
// Upper bits reserved for future attributes.
const uint16_t UART_PIXEL_BYTE_PARITY_CHECK = 0x80;
const uint16_t UART_PIXEL_FORMAT_RGB565 = 0x01 | UART_PIXEL_BYTE_PARITY_CHECK; // Signal to ArduImageCapture that parity check is enabled
const uint16_t UART_PIXEL_FORMAT_GRAYSCALE = 0x02 | UART_PIXEL_BYTE_PARITY_CHECK; // Signal to ArduImageCapture that parity check is enabled;

// Pixel byte parity check:
// Pixel Byte H: odd number of bits under H_BYTE_PARITY_CHECK and H_BYTE_PARITY_INVERT
// Pixel Byte L: even number of bits under L_BYTE_PARITY_CHECK and L_BYTE_PARITY_INVERT
//                                          H:RRRRRGGG
const uint8_t H_BYTE_PARITY_CHECK =  0b00100000;
const uint8_t H_BYTE_PARITY_INVERT = 0b00001000;
//                                          L:GGGBBBBB
const uint8_t L_BYTE_PARITY_CHECK =  0b00001000;
const uint8_t L_BYTE_PARITY_INVERT = 0b00100000;
// Since the parity for L byte can be zero we must ensure that the total byet value is above zero.
// Increasing the lowest bit of blue color is OK for that.
const uint8_t L_BYTE_PREVENT_ZERO  = 0b00000001;


const uint16_t COLOR_GREEN = 0x07E0;
const uint16_t COLOR_RED = 0xF800;



void processGrayscaleFrameBuffered();
void processGrayscaleFrameDirect();
void processRgbFrameBuffered();
void processRgbFrameDirect();
typedef void (*ProcessFrameData)(void) ;


#if UART_MODE==1
const uint16_t lineLength = 160;
const uint16_t lineCount = 120;
const uint32_t baud  = 115200;
const ProcessFrameData processFrameData = processRgbFrameBuffered;
const uint16_t lineBufferLength = lineLength * 2;
const bool isSendWhileBuffering = true;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_RGB565, 34);
#endif

#if UART_MODE==2
const uint16_t lineLength = 160;
const uint16_t lineCount = 120;
const uint32_t baud  = 115200;
const ProcessFrameData processFrameData = processGrayscaleFrameBuffered;
const uint16_t lineBufferLength = lineLength;
const bool isSendWhileBuffering = true;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_YUV422, 17);
#endif

#if UART_MODE==3
const uint16_t lineLength = 160;
const uint16_t lineCount = 120;
const uint32_t baud  = 500000;
const ProcessFrameData processFrameData = processRgbFrameBuffered;
const uint16_t lineBufferLength = lineLength * 2;
const bool isSendWhileBuffering = true;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_RGB565, 8);
#endif

#if UART_MODE==4
const uint16_t lineLength = 160;
const uint16_t lineCount = 120;
const uint32_t baud  = 500000;
const ProcessFrameData processFrameData = processGrayscaleFrameBuffered;
const uint16_t lineBufferLength = lineLength;
const bool isSendWhileBuffering = true;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_YUV422, 4);
#endif

#if UART_MODE==5
const uint16_t lineLength = 320;
const uint16_t lineCount = 240;
const uint32_t baud  = 500000;
const ProcessFrameData processFrameData = processRgbFrameBuffered;
const uint16_t lineBufferLength = lineLength * 2;
const bool isSendWhileBuffering = true;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QVGA_320x240, CameraOV7670::PIXEL_RGB565, 32);
#endif

#if UART_MODE==6
const uint16_t lineLength = 320;
const uint16_t lineCount = 240;
const uint32_t baud  = 500000;
const ProcessFrameData processFrameData = processGrayscaleFrameBuffered;
const uint16_t lineBufferLength = lineLength;
const bool isSendWhileBuffering = true;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QVGA_320x240, CameraOV7670::PIXEL_YUV422, 16);
#endif

#if UART_MODE==7
const uint16_t lineLength = 160;
const uint16_t lineCount = 120;
const uint32_t baud  = 1000000;
const ProcessFrameData processFrameData = processRgbFrameBuffered;
const uint16_t lineBufferLength = lineLength * 2;
const bool isSendWhileBuffering = false;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_RGB565, 5);
#endif

#if UART_MODE==8
const uint16_t lineLength = 160;
const uint16_t lineCount = 120;
const uint32_t baud  = 1000000;
const ProcessFrameData processFrameData = processGrayscaleFrameBuffered;
const uint16_t lineBufferLength = lineLength;
const bool isSendWhileBuffering = false;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_YUV422, 2);
#endif

#if UART_MODE==9
const uint16_t lineLength = 320;
const uint16_t lineCount = 240;
const uint32_t baud  = 1000000;
const ProcessFrameData processFrameData = processRgbFrameBuffered;
const uint16_t lineBufferLength = lineLength * 2;
const bool isSendWhileBuffering = true;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QVGA_320x240, CameraOV7670::PIXEL_RGB565, 16);
#endif

#if UART_MODE==10
const uint16_t lineLength = 320;
const uint16_t lineCount = 240;
const uint32_t baud  = 1000000;
const ProcessFrameData processFrameData = processGrayscaleFrameBuffered;
const uint16_t lineBufferLength = lineLength;
const bool isSendWhileBuffering = true;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QVGA_320x240, CameraOV7670::PIXEL_YUV422, 8);
#endif

#if UART_MODE==11
const uint16_t lineLength = 640;
const uint16_t lineCount = 480;
const uint32_t baud  = 1000000;
const ProcessFrameData processFrameData = processGrayscaleFrameDirect;
const uint16_t lineBufferLength = 1;
const bool isSendWhileBuffering = true;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_VGA_640x480, CameraOV7670::PIXEL_YUV422, 39);
#endif

#if UART_MODE==12
const uint16_t lineLength = 160;
const uint16_t lineCount = 120;
const uint32_t baud  = 2000000;
const ProcessFrameData processFrameData = processRgbFrameBuffered;
const uint16_t lineBufferLength = lineLength * 2;
const bool isSendWhileBuffering = false;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_RGB565, 2);
#endif

#if UART_MODE==13
const uint16_t lineLength = 160;
const uint16_t lineCount = 120;
const uint32_t baud  = 2000000;
const ProcessFrameData processFrameData = processGrayscaleFrameBuffered;
const uint16_t lineBufferLength = lineLength;
const bool isSendWhileBuffering = false;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QQVGA_160x120, CameraOV7670::PIXEL_YUV422, 2);
#endif

#if UART_MODE==14
const uint16_t lineLength = 320;
const uint16_t lineCount = 240;
const uint32_t baud  = 2000000; // may be unreliable
const ProcessFrameData processFrameData = processRgbFrameBuffered;
const uint16_t lineBufferLength = lineLength * 2;
const bool isSendWhileBuffering = true;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QVGA_320x240, CameraOV7670::PIXEL_RGB565, 11);
#endif

#if UART_MODE==15
const uint16_t lineLength = 320;
const uint16_t lineCount = 240;
const uint32_t baud  = 2000000; // may be unreliable
const ProcessFrameData processFrameData = processGrayscaleFrameBuffered;
const uint16_t lineBufferLength = lineLength;
const bool isSendWhileBuffering = false;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_QVGA_320x240, CameraOV7670::PIXEL_YUV422, 6);
#endif

#if UART_MODE==16
const uint16_t lineLength = 640;
const uint16_t lineCount = 480;
const uint32_t baud  = 2000000;
const ProcessFrameData processFrameData = processRgbFrameDirect;
const uint16_t lineBufferLength = 1;
const bool isSendWhileBuffering = true;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_RGB565;
CameraOV7670 camera(CameraOV7670::RESOLUTION_VGA_640x480, CameraOV7670::PIXEL_RGB565, 39);
#endif

#if UART_MODE==17
const uint16_t lineLength = 640;
const uint16_t lineCount = 480;
const uint32_t baud  = 2000000;
const ProcessFrameData processFrameData = processGrayscaleFrameDirect;
const uint16_t lineBufferLength = 1;
const bool isSendWhileBuffering = true;
const uint8_t uartPixelFormat = UART_PIXEL_FORMAT_GRAYSCALE;
CameraOV7670 camera(CameraOV7670::RESOLUTION_VGA_640x480, CameraOV7670::PIXEL_YUV422, 19);
#endif


uint8_t lineBuffer [lineBufferLength]; // Two bytes per pixel
uint8_t * lineBufferSendByte;
bool isLineBufferSendHighByte;
bool isLineBufferByteFormatted;

uint16_t frameCounter = 0;
uint16_t processedByteCountDuringCameraRead = 0;



void sendBlankFrame(uint16_t color);
inline void startNewFrame(uint8_t pixelFormat) __attribute__((always_inline));
inline void endOfLine(void) __attribute__((always_inline));
inline void processNextGrayscalePixelByteInBuffer() __attribute__((always_inline));
inline void processNextRgbPixelByteInBuffer() __attribute__((always_inline));
inline void tryToSendNextRgbPixelByteInBuffer() __attribute__((always_inline));
inline void formatNextRgbPixelByteInBuffer() __attribute__((always_inline));
inline uint8_t formatRgbPixelByteH(uint8_t byte) __attribute__((always_inline));
inline uint8_t formatRgbPixelByteL(uint8_t byte) __attribute__((always_inline));
inline uint8_t formatPixelByteGrayscaleFirst(uint8_t byte) __attribute__((always_inline));
inline uint8_t formatPixelByteGrayscaleSecond(uint8_t byte) __attribute__((always_inline));
inline void waitForPreviousUartByteToBeSent() __attribute__((always_inline));
inline bool isUartReady() __attribute__((always_inline));

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
      waitForPreviousUartByteToBeSent();
      UDR0 = formatRgbPixelByteH(colorH);
      waitForPreviousUartByteToBeSent();
      UDR0 = formatRgbPixelByteL(colorL);
    }
    endOfLine();
  }
}




// this is called in Arduino loop() function
void processFrame() {
  processedByteCountDuringCameraRead = 0;
  startNewFrame(uartPixelFormat);
  noInterrupts();
  processFrameData();
  interrupts();
  frameCounter++;
  debugPrint("Frame " + String(frameCounter) + " " + String(processedByteCountDuringCameraRead));
  //debugPrint("Frame " + String(frameCounter, 16)); // send number in hexadecimal
}


void processGrayscaleFrameBuffered() {
  camera.waitForVsync();
  camera.ignoreVerticalPadding();

  for (uint16_t y = 0; y < lineCount; y++) {
    lineBufferSendByte = &lineBuffer[0];
    camera.ignoreHorizontalPaddingLeft();
    isLineBufferSendHighByte = true;

    for (uint16_t x = 0; x < lineBufferLength; x++) {
      camera.waitForPixelClockRisingEdge(); // YUV422 grayscale byte
      camera.readPixelByte(lineBuffer[x]);
      lineBuffer[x] = formatPixelByteGrayscaleFirst(lineBuffer[x]);

      camera.waitForPixelClockRisingEdge(); // YUV422 color byte. Ignore.
      if (isSendWhileBuffering) {
        processNextGrayscalePixelByteInBuffer();
      }

      x++;
      camera.waitForPixelClockRisingEdge(); // YUV422 grayscale byte
      camera.readPixelByte(lineBuffer[x]);
      lineBuffer[x] = formatPixelByteGrayscaleSecond(lineBuffer[x]);

      camera.waitForPixelClockRisingEdge(); // YUV422 color byte. Ignore.
      if (isSendWhileBuffering) {
        processNextGrayscalePixelByteInBuffer();
      }
    }
    camera.ignoreHorizontalPaddingRight();

    // Debug info to get some feedback how mutch data was processed during line read.
    processedByteCountDuringCameraRead = lineBufferSendByte - (&lineBuffer[0]);

    // Send rest of the line
    while (lineBufferSendByte < &lineBuffer[lineLength]) {
      processNextGrayscalePixelByteInBuffer();
    }
 
    endOfLine();
  };
}

void processNextGrayscalePixelByteInBuffer() {
  if (isUartReady()) {
    UDR0 = *lineBufferSendByte;
    lineBufferSendByte++;        
  }
}


void processGrayscaleFrameDirect() {
  camera.waitForVsync();
  camera.ignoreVerticalPadding();
  
  for (uint16_t y = 0; y < lineCount; y++) {
    camera.ignoreHorizontalPaddingLeft();

    for (uint16_t x = 0; x < lineLength / 2; x++) {
      camera.waitForPixelClockRisingEdge(); // YUV422 grayscale byte
      camera.readPixelByte(lineBuffer[0]);
      lineBuffer[0] = formatPixelByteGrayscaleFirst(lineBuffer[0]);
      
      camera.waitForPixelClockRisingEdge(); // YUV422 color byte. Ignore.
      waitForPreviousUartByteToBeSent();
      UDR0 = lineBuffer[0];

      camera.waitForPixelClockRisingEdge(); // YUV422 grayscale byte
      camera.readPixelByte(lineBuffer[0]);
      lineBuffer[0] = formatPixelByteGrayscaleSecond(lineBuffer[0]);
      
      camera.waitForPixelClockRisingEdge(); // YUV422 color byte. Ignore.
      waitForPreviousUartByteToBeSent();
      UDR0 = lineBuffer[0];
    }

    camera.ignoreHorizontalPaddingRight();
    endOfLine();
  }
}

uint8_t formatPixelByteGrayscaleFirst(uint8_t pixelByte) {  
  // Make pixel color always slightly above 0 since zero is end of line marker
  pixelByte &= 0b11111110;
  if (pixelByte == 0) {
    pixelByte |= 0b00000010;
  }
  return pixelByte;
}

uint8_t formatPixelByteGrayscaleSecond(uint8_t pixelByte) {
   // Make pixel color always slightly above 0 since zero is end of line marker
  return pixelByte | 0b00000001;
}



void processRgbFrameBuffered() {
  camera.waitForVsync();
  camera.ignoreVerticalPadding();
  
  for (uint16_t y = 0; y < lineCount; y++) {
    lineBufferSendByte = &lineBuffer[0];
    isLineBufferSendHighByte = true; // Line starts with High byte
    isLineBufferByteFormatted = false;

    camera.ignoreHorizontalPaddingLeft();

    for (uint16_t x = 0; x < lineLength * 2;  x++) {
      camera.waitForPixelClockRisingEdge();
      camera.readPixelByte(lineBuffer[x]);
      if (isSendWhileBuffering) {
        processNextRgbPixelByteInBuffer();
      }
    };

    camera.ignoreHorizontalPaddingRight();

    // Debug info to get some feedback how mutch data was processed during line read.
    processedByteCountDuringCameraRead = lineBufferSendByte - (&lineBuffer[0]);

    // send rest of the line
    while (lineBufferSendByte < &lineBuffer[lineLength * 2]) {
      processNextRgbPixelByteInBuffer();
    }

    endOfLine();
  }
}

void processNextRgbPixelByteInBuffer() {
  // Format pixel bytes and send out in different cycles.
  // There is not enough time to do both on faster frame rates.
  if (isLineBufferByteFormatted) {
    tryToSendNextRgbPixelByteInBuffer();
  } else {
    formatNextRgbPixelByteInBuffer();
  }        
}

void tryToSendNextRgbPixelByteInBuffer() {
  if (isUartReady()) {
    UDR0 = *lineBufferSendByte;
    lineBufferSendByte++;    
    isLineBufferByteFormatted = false;    
  }
}

void formatNextRgbPixelByteInBuffer() {
  if (isLineBufferSendHighByte) {
    *lineBufferSendByte = formatRgbPixelByteH(*lineBufferSendByte);
  } else {
    *lineBufferSendByte = formatRgbPixelByteL(*lineBufferSendByte);
  }
  isLineBufferByteFormatted = true;
  isLineBufferSendHighByte = !isLineBufferSendHighByte;
}




void processRgbFrameDirect() {
  camera.waitForVsync();
  camera.ignoreVerticalPadding();

  for (uint16_t y = 0; y < lineCount; y++) {
    camera.ignoreHorizontalPaddingLeft();
    
    for (uint16_t x = 0; x < lineLength; x++) {
      
      camera.waitForPixelClockRisingEdge();
      camera.readPixelByte(lineBuffer[0]);
      lineBuffer[0] = formatRgbPixelByteH(lineBuffer[0]);
      waitForPreviousUartByteToBeSent();
      UDR0 = lineBuffer[0];
      
      camera.waitForPixelClockRisingEdge();
      camera.readPixelByte(lineBuffer[0]);
      lineBuffer[0] = formatRgbPixelByteL(lineBuffer[0]);
      waitForPreviousUartByteToBeSent();
      UDR0 = lineBuffer[0];
    }
    
    camera.ignoreHorizontalPaddingRight();
    endOfLine();
  };
}


// RRRRRGGG
uint8_t formatRgbPixelByteH(uint8_t pixelByteH) {
  // Make sure that
  // A: pixel color always slightly above 0 since zero is end of line marker
  // B: odd number of bits for H byte under H_BYTE_PARITY_CHECK and H_BYTE_PARITY_INVERT to enable error correction
  if (pixelByteH & H_BYTE_PARITY_CHECK) {
    return pixelByteH & (~H_BYTE_PARITY_INVERT);
  } else {
    return pixelByteH | H_BYTE_PARITY_INVERT;
  }
}


// GGGBBBBB
uint8_t formatRgbPixelByteL(uint8_t pixelByteL) {
  // Make sure that
  // A: pixel color always slightly above 0 since zero is end of line marker
  // B: even number of bits for L byte under L_BYTE_PARITY_CHECK and L_BYTE_PARITY_INVERT to enable error correction
  if (pixelByteL & L_BYTE_PARITY_CHECK) {
    return pixelByteL | L_BYTE_PARITY_INVERT | L_BYTE_PREVENT_ZERO;
  } else {
    return (pixelByteL & (~L_BYTE_PARITY_INVERT)) | L_BYTE_PREVENT_ZERO;
  }
}









void startNewFrame(uint8_t pixelFormat) {
  waitForPreviousUartByteToBeSent();
  UDR0 = 0x00;
  waitForPreviousUartByteToBeSent();
  UDR0 = COMMAND_NEW_FRAME;

  // frame width
  waitForPreviousUartByteToBeSent();
  UDR0 = (lineLength >> 8) & 0xFF;
  waitForPreviousUartByteToBeSent();
  UDR0 = lineLength & 0xFF;

  // frame height
  waitForPreviousUartByteToBeSent();
  UDR0 = (lineCount >> 8) & 0xFF;
  waitForPreviousUartByteToBeSent();
  UDR0 = lineCount & 0xFF;

  // pixel format
  waitForPreviousUartByteToBeSent();
  UDR0 = (pixelFormat);
}



void endOfLine()   {
  waitForPreviousUartByteToBeSent();
  UDR0 = 0x00;
  waitForPreviousUartByteToBeSent();
  UDR0 = COMMAND_END_OF_LINE;
}



void debugPrint(const String debugText) {
    waitForPreviousUartByteToBeSent();
    UDR0 = 0x00;
    waitForPreviousUartByteToBeSent();
    UDR0 = COMMAND_DEBUG_DATA;
    waitForPreviousUartByteToBeSent();
    UDR0 = debugText.length();
    for (uint16_t i=0; i<debugText.length(); i++) {
        waitForPreviousUartByteToBeSent();
        UDR0 = debugText[i];
    }
}



void waitForPreviousUartByteToBeSent() {
  while(!isUartReady()); //wait for byte to transmit
}


bool isUartReady() {
  return UCSR0A & (1<<UDRE0);
}


#endif
