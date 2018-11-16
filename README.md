# LiveOV7670
  
Demo:  
https://www.youtube.com/watch?v=TqSY6FETuos&list=PLVilroPGLJaesaS5mP93i0goPck2JK-O4&index=2
Datasheet for OV7670:  
https://www.voti.nl/docs/OV7670.pdf

-------------------------------------------------------------------------------  
  
Compiling in Arduino IDE:  
  
1. Download all files
2. copy "src/lib/LiveOV7670Library" and "src/lib/Adafruit_GFX_Library" to Arduino "libraries" folder (If you already have "Adafruit_GFX_Library" then you don't have to copy that)
3. Open "src/LiveOV7670/LiveOV7670.ino" in Arduino IDE
4. Select Tools->Board->Arduino Uno/Nano
  
-------------------------------------------------------------------------------
  
https://raw.githubusercontent.com/indrekluuk/LiveOV7670/master/LiveOV7670.png

OV7670 connections:  
  
VSYNC - PIN2  
XCLCK - PIN3 (must be level shifted from 5V -> 3.3V)  
PCLCK - PIN12  
SIOD  - A4 (I2C data) - 10K resistor to 3.3V  
SIOC  - A5 (I2C clock) - 10K resistor to 3.3V  
D0..D3 - A0..A3 (pixel data bits 0..3)  
D4..D7 - PIN4..PIN7 (pixel data bits 4..7)  
3.3V  - 3.3V  
RESET - 3.3V  
GND   - GND  
PWDN  - GND  
  
1.8" TFT connections:  
  
DC - PIN 8 (5V -> 3.3V)  
CS - PIN 9 (5V -> 3.3V)  
RESET - PIN 10 (5V -> 3.3V)  
SPI data - PIN 11 (5V -> 3.3V)  
SPI clock - PIN 13 (5V -> 3.3V)  
VCC - 5V/3.3V (depending on jumper position on the TFT board)  
BL - 3.3V  
GND - GND  
  
-------------------------------------------------------------------------------
  
Special thanks to
https://github.com/ComputerNerd/ov7670-no-ram-arduino-uno

