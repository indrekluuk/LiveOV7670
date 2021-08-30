//
// Created by indrek on 30.04.2016.
//

#ifndef _CAMERAOV7670LINEBUFFER_H
#define _CAMERAOV7670LINEBUFFER_H

#include "CameraOV7670.h"



// Pixel receiving order from LiveOV7670Library for downsampled pictures: Pixel_1_H, Pixel_1_L, Pixel_2_H, Pixel_2_L ...
// First byte from LiveOV7670Library is half a pixel (higher byte of first pixel).
// Shift line data by 1 byte to correct for it.
// This means that first pixel in each line is actually broken.
template <typename TBuffer, TBuffer size>
union OV7670PixelBuffer {
  struct {
    uint8_t writeBufferPadding;
    uint8_t writeBuffer[size];
  };
  struct {
    uint8_t readBuffer[size];
    uint8_t readBufferPadding;
  };
};




// TBuffer type for buffer size. If buffer is smaller than 256 then uin8_t can be used otherwise use uin16_t
// Tx type for line length. If line length is smaller than 256 then uin8_t can be used otherwise use uin16_t
// Ty type for line count. If line length is smaller than 256 then uin8_t can be used otherwise use uin16_t
template <typename TBuffer, TBuffer bufferLength, typename Tx, Tx lineLength, typename Ty, Ty lineCount>
class BufferedCameraOV7670 : public CameraOV7670 {

protected:
  static OV7670PixelBuffer<TBuffer, bufferLength> pixelBuffer;

public:
  BufferedCameraOV7670(Resolution resolution, PixelFormat format, uint8_t internalClockPreScaler, PLLMultiplier pllMultiplier = PLL_MULTIPLIER_BYPASS) :
      CameraOV7670(resolution, format, internalClockPreScaler, pllMultiplier) {};

  virtual void readLine();

  inline static constexpr Tx getLineLength() __attribute__((always_inline));
  inline static constexpr Ty getLineCount() __attribute__((always_inline));
  inline const uint8_t * getPixelBuffer() __attribute__((always_inline));
  inline static constexpr TBuffer getPixelBufferLength() __attribute__((always_inline));
  inline const uint8_t getPixelByte(TBuffer byteIndex) __attribute__((always_inline));


};



template <typename TBuffer, TBuffer bufferLength, typename Tx, Tx lineLength, typename Ty, Ty lineCount>
OV7670PixelBuffer<TBuffer, bufferLength> BufferedCameraOV7670<TBuffer, bufferLength, Tx, lineLength, Ty, lineCount>::pixelBuffer;




template <typename TBuffer, TBuffer bufferLength, typename Tx, Tx lineLength, typename Ty, Ty lineCount>
constexpr Tx BufferedCameraOV7670<TBuffer, bufferLength, Tx, lineLength, Ty, lineCount>::getLineLength() {
  return lineLength;
}


template <typename TBuffer, TBuffer bufferLength, typename Tx, Tx lineLength, typename Ty, Ty lineCount>
constexpr Ty BufferedCameraOV7670<TBuffer, bufferLength, Tx, lineLength, Ty, lineCount>::getLineCount() {
  return lineCount;
}


template <typename TBuffer, TBuffer bufferLength, typename Tx, Tx lineLength, typename Ty, Ty lineCount>
const uint8_t * BufferedCameraOV7670<TBuffer, bufferLength, Tx, lineLength, Ty, lineCount>::getPixelBuffer() {
  return pixelBuffer.readBuffer;
};


template <typename TBuffer, TBuffer bufferLength, typename Tx, Tx lineLength, typename Ty, Ty lineCount>
constexpr TBuffer BufferedCameraOV7670<TBuffer, bufferLength, Tx, lineLength, Ty, lineCount>::getPixelBufferLength() {
  return bufferLength;
}


template <typename TBuffer, TBuffer bufferLength, typename Tx, Tx lineLength, typename Ty, Ty lineCount>
const uint8_t BufferedCameraOV7670<TBuffer, bufferLength, Tx, lineLength, Ty, lineCount>::getPixelByte(const TBuffer byteIndex) {
  return pixelBuffer.readBuffer[byteIndex];
}


template <typename TBuffer, TBuffer bufferLength, typename Tx, Tx lineLength, typename Ty, Ty lineCount>
void BufferedCameraOV7670<TBuffer, bufferLength, Tx, lineLength, Ty, lineCount>::readLine() {

  pixelBuffer.writeBufferPadding = 0;
  TBuffer bufferIndex = 0;

  while (bufferIndex < getPixelBufferLength()) {
    waitForPixelClockRisingEdge();
    readPixelByte(pixelBuffer.writeBuffer[bufferIndex++]);
    waitForPixelClockRisingEdge();
    readPixelByte(pixelBuffer.writeBuffer[bufferIndex++]);
  }
}








#endif //_CAMERAOV7670LINEBUFFER_H
