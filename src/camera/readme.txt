
OV7670 PIN connections:

VSYNC - PIN2
XCLCK - PIN3 (must be level shifted from 5V -> 3.3V)
PCLCK - PIN12
SIOD - A4 (I2C data) - 10K resistor to 3.3V
SIOC - A5 (I2C clock) - 10K resistor to 3.3V
D0..D3 - A0..A3 (pixel data bits 0..3)
D4..D7 - PIN4..PIN7 (pixel data bits 4..7)
3.3V - 3.3V
RESET - 3.3V
GND - GND
PWDN - GND
