//
// Created by indrek on 29.10.2016.
//


#include "gtest/gtest.h"
#include "OV7670Simulator.h"
#include "LiveOV7670Library/CameraOV7670.h"





TEST(TestCameraOV7670, testWaitForVsync) {
  ov7670Simulator = OV7670Simulator();
  CameraOV7670 cameraOV7670(CameraOV7670::RESOLUTION_VGA_640x480, CameraOV7670::PIXEL_RGB565, 0);

  int vsyncCheckCount = 0;
  ov7670Simulator.setIsVsyncCallback([&vsyncCheckCount]() {
      vsyncCheckCount++;
      return vsyncCheckCount > 4;
  });

  cameraOV7670.waitForVsync();

  EXPECT_EQ(5, vsyncCheckCount);
}






TEST(TestCameraOV7670, testWaitForPixelClockRisingEdge) {
  ov7670Simulator = OV7670Simulator();
  CameraOV7670 cameraOV7670(CameraOV7670::RESOLUTION_VGA_640x480, CameraOV7670::PIXEL_RGB565, 0);

  int pixelClockCheckCount = 0;
  ov7670Simulator.setIsPixelClockCallback([&pixelClockCheckCount]() {
      pixelClockCheckCount++;
      return (pixelClockCheckCount < 10) || (pixelClockCheckCount > 13);
  });

  cameraOV7670.waitForPixelClockRisingEdge();

  EXPECT_EQ(14, pixelClockCheckCount);
}






