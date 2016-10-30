//
// Created by indrek on 30.10.2016.
//



#include "gtest/gtest.h"
#include "OV7670Simulator.h"
#include "camera/buffered/BufferedCameraOV7670_QVGA.h"
#include "camera/buffered/BufferedCameraOV7670_QQVGA.h"
#include "camera/buffered/BufferedCameraOV7670_QQVGA_10hz.h"




static const uint8_t PIXEL_VALUE_START = 100;
static const uint8_t PIXEL_VALUE_END = 199;

template <int x, int y>
void testBufferedCameraOV7670(BufferedCameraOV7670<x, y> & camera) {
  // re-initialize to guarantee clean setup
  ov7670Simulator = OV7670Simulator();

  // flip-flop pixel clock after every check
  bool pixelClockFlipFlop = true;
  ov7670Simulator.setIsPixelClockCallback([&pixelClockFlipFlop](){
      pixelClockFlipFlop = !pixelClockFlipFlop;
      return pixelClockFlipFlop;
  });

  // generate pixel with range of 100..199
  uint8_t pixelValueCounter = PIXEL_VALUE_START;
  ov7670Simulator.setGetPixelCallback([&pixelValueCounter](){
      uint8_t pixelByte = pixelValueCounter;
      if (++pixelValueCounter>PIXEL_VALUE_END) pixelValueCounter = PIXEL_VALUE_START;
      return pixelByte;
  });


  camera.readLine();


  ASSERT_EQ(x, camera.getLineLength());

  // pixel order in the buffer is HIGH_1, LOW_1, HIGH_2, LOW_2, HIGH_3, LOW_3 ...
  // when camera sends data it starts with LOW_1 instead on HIGH_1
  ASSERT_EQ(0, camera.getPixelByte(0));
  ASSERT_EQ(PIXEL_VALUE_START, camera.getPixelByte(1));
  ASSERT_EQ(PIXEL_VALUE_START+1, camera.getPixelByte(2));
  ASSERT_EQ(PIXEL_VALUE_START+2, camera.getPixelByte(3));
  ASSERT_EQ(x%(PIXEL_VALUE_END+1-PIXEL_VALUE_START)+PIXEL_VALUE_START-2, camera.getPixelByte(x-1));
}





TEST(TestBufferedCameraOV7670, testQVGA_minFps) {
  BufferedCameraOV7670_QVGA camera(CameraOV7670::PIXEL_RGB565, BufferedCameraOV7670_QVGA::FPS_1p25_Hz);
  testBufferedCameraOV7670<320, 240>(camera);
}

TEST(TestBufferedCameraOV7670, testQVGA_maxFps) {
  BufferedCameraOV7670_QVGA camera(CameraOV7670::PIXEL_RGB565, BufferedCameraOV7670_QVGA::FPS_2p5_Hz);
  testBufferedCameraOV7670<320, 240>(camera);
}

TEST(TestBufferedCameraOV7670, testQQVGA_minFps) {
  BufferedCameraOV7670_QQVGA camera(CameraOV7670::PIXEL_RGB565, BufferedCameraOV7670_QQVGA::FPS_1p66_Hz);
  testBufferedCameraOV7670<160, 120>(camera);
}

TEST(TestBufferedCameraOV7670, testQQVGA_maxFps) {
  BufferedCameraOV7670_QQVGA camera(CameraOV7670::PIXEL_RGB565, BufferedCameraOV7670_QQVGA::FPS_5_Hz);
  testBufferedCameraOV7670<160, 120>(camera);
}

TEST(TestBufferedCameraOV7670, testQQVGA_FPS_10_Hz) {
  BufferedCameraOV7670_QQVGA_10hz camera(CameraOV7670::PIXEL_RGB565);
  testBufferedCameraOV7670<160, 120>(camera);
}





