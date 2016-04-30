# LiveOV7670


Compiling in Arduino IDE:  
  
1. Download all files  
2. If you don't already have "Adafruit_GFX_Library" in your Arduino "libraries" folder then copy it from "lib" folder (or download from Adafruit) to Arduino "libraries" folder
3. Rename "src" folder to "LiveOV7670"
4. Open "LiveOV7670.ino" in Arduino IDE
5. Select Tools->Board->Arduino Nano
  
-------------------------------------------------------------------------------
  
Arduino to OV7670 PIN connections:  
  
PIN2  - Vertical sync in  
PIN3  - Clock out  
PIN12 - Pixel Clock in  
A4    - I2C data  
A5    - I2C clock  
A0..A3     - Pixel data bits 0..3  
PIN4..PIN7 - Pixel data bits 4..7  
  
Arduino to TFT PIN connections:  
  
PIN 8  - DC  
PIN 9  - CS  
PIN 10 - RESET  
PIN 11 - SPI data in  
PIN 13 - SPI clock out  
  
-------------------------------------------------------------------------------
  
Special thanks to
https://github.com/ComputerNerd/ov7670-no-ram-arduino-uno

