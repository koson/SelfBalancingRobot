/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/
#include <SPI.h>
#include <Wire.h>
#include "display_driver.h"

 /****************************************************************************************
 * HARDWARE PORTS ASSIGNMENTS
 ****************************************************************************************/



/****************************************************************************************
 * GLOBAL VARIABLES
 ***************************************************************************************/


/****************************************************************************************
 * DEFINITIONS
 ****************************************************************************************/
#define BOWSERH  37
#define BOWSERW  40
#define DELAYBOWSER 100


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

 
/****************************************************************************************
 * STATIC VARIABLES
 ****************************************************************************************/
static bool display_driver_initialized             = false;
static bool driver_in_testing_mode                 = false;         // This boolean should be true only in the testing and debugging process

static uint8_t PROGMEM frame_bitmap[128];


static const unsigned char PROGMEM Bowser_1[] = {
0x01, 0x00, 0x98, 0x00, 0x00, 0x02, 0x81, 0x54, 0x00, 0x00, 0x02, 0x8B, 0x73, 0x20, 0x00, 0x04,
0x94, 0x12, 0xD0, 0x00, 0x04, 0xE0, 0x11, 0x38, 0x00, 0x04, 0xE3, 0xE3, 0x74, 0x00, 0x04, 0xFF,
0xE3, 0x08, 0x00, 0x0F, 0xFF, 0xF6, 0x10, 0x00, 0x13, 0xFC, 0x7E, 0x14, 0x00, 0x23, 0xF8, 0x1F,
0x0A, 0x00, 0x49, 0xF7, 0xBF, 0x04, 0x00, 0x3C, 0xC9, 0xFF, 0x84, 0x00, 0x1A, 0xC8, 0xCF, 0xC4,
0x00, 0x18, 0xF1, 0x87, 0x83, 0x00, 0x1F, 0xCF, 0x03, 0xE3, 0xC0, 0x30, 0xEE, 0x23, 0x91, 0xD0,
0x60, 0x3C, 0xF3, 0x0D, 0xF8, 0x40, 0x01, 0xB3, 0x0F, 0x38, 0xD8, 0xC3, 0xF7, 0x03, 0xFC, 0x88,
0x83, 0xF7, 0x80, 0xFE, 0x80, 0x0E, 0xF7, 0xFC, 0x7F, 0x80, 0x1F, 0xEF, 0xFE, 0x3F, 0xC7, 0x39,
0x8F, 0xFF, 0x1F, 0x7F, 0xF1, 0x9F, 0xFF, 0x9F, 0x49, 0xF1, 0x1F, 0xFF, 0xCF, 0x29, 0xF9, 0x1F,
0xFF, 0xCF, 0x1A, 0xFA, 0x3F, 0xFF, 0xE7, 0x03, 0x18, 0x3F, 0xFF, 0xE7, 0x02, 0x00, 0x7F, 0xFF,
0xE7, 0x03, 0x01, 0xFF, 0xFF, 0xE7, 0x05, 0xFF, 0xFE, 0xD7, 0xE7, 0x07, 0xFF, 0xFF, 0xFF, 0xE7,
0x07, 0xFF, 0xFF, 0xFE, 0x00, 0x07, 0xFE, 0x07, 0xFE, 0x00, 0x04, 0x92, 0x04, 0x92, 0x00, 0x02,
0xDA, 0x02, 0xD4, 0x00, 0x01, 0xB6, 0x01, 0xA8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

static const unsigned char PROGMEM Bowser_2[] = {
0x01, 0x00, 0x98, 0x00, 0x00, 0x02, 0x81, 0x54, 0x00, 0x00, 0x02, 0x8B, 0x73, 0x20, 0x00, 0x04,
0x94, 0x12, 0xD0, 0x00, 0x04, 0xE0, 0x11, 0x28, 0x00, 0x04, 0xE3, 0xE3, 0x74, 0x00, 0x04, 0xFF,
0xE3, 0x08, 0x00, 0x0F, 0xFF, 0xF6, 0x10, 0x00, 0x17, 0xFE, 0x7E, 0x14, 0x00, 0x23, 0xFC, 0x1F,
0x0A, 0x00, 0x4B, 0xF8, 0x3F, 0x04, 0x00, 0x39, 0xF9, 0xFF, 0x84, 0x00, 0x1D, 0xEE, 0xCF, 0xC4,
0x00, 0x1A, 0xC9, 0x87, 0x83, 0x00, 0x1F, 0xF7, 0x03, 0xE3, 0xC0, 0x30, 0xE6, 0x23, 0x91, 0xD0,
0x60, 0x3C, 0xF3, 0x0D, 0xF8, 0x40, 0x01, 0xB3, 0x0F, 0x38, 0xD8, 0xC3, 0xF7, 0x03, 0xFC, 0x88,
0x83, 0xF7, 0x80, 0xFE, 0x80, 0x0E, 0xF7, 0xFC, 0x7F, 0x80, 0x1F, 0xEF, 0xFE, 0x3F, 0xC7, 0x39,
0x8F, 0xFF, 0x1F, 0x7F, 0xF1, 0x9F, 0xFF, 0x9F, 0x49, 0xF1, 0x1F, 0xFF, 0xCF, 0x29, 0xF9, 0x1F,
0xFF, 0xCF, 0x1A, 0xFA, 0x3F, 0xFF, 0xE7, 0x03, 0x18, 0x3F, 0xFF, 0xE7, 0x02, 0x00, 0x7F, 0xFF,
0xE7, 0x03, 0x01, 0xFF, 0xFF, 0xE7, 0x05, 0xFF, 0xFE, 0xD7, 0xE7, 0x07, 0xFF, 0xFF, 0xFF, 0xE7,
0x07, 0xFF, 0x07, 0xFE, 0x00, 0x07, 0xFE, 0x07, 0xFE, 0x00, 0x04, 0x92, 0x04, 0x92, 0x00, 0x02,
0xDA, 0x02, 0xD4, 0x00, 0x01, 0xB6, 0x01, 0xA8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

static const unsigned char PROGMEM Bowser_3[] = {
0x01, 0x00, 0x98, 0x00, 0x00, 0x02, 0x81, 0x54, 0x00, 0x00, 0x02, 0x8B, 0x73, 0x20, 0x00, 0x04,
0x94, 0x12, 0xD0, 0x00, 0x04, 0xE0, 0x11, 0x28, 0x00, 0x04, 0xE3, 0xE3, 0x74, 0x00, 0x04, 0xFF,
0xE3, 0x08, 0x00, 0x0F, 0xFF, 0xF6, 0x10, 0x00, 0x17, 0xFE, 0x7E, 0x14, 0x00, 0x23, 0xFC, 0x1F,
0x0A, 0x00, 0x4B, 0xF8, 0x3F, 0x04, 0x00, 0x39, 0xF9, 0xCF, 0x84, 0x00, 0x1D, 0xEE, 0x87, 0xC4,
0x00, 0x1A, 0xCF, 0x07, 0x83, 0x00, 0x1F, 0xF4, 0x23, 0xE3, 0xC0, 0x30, 0xE8, 0x73, 0x91, 0xD0,
0x60, 0x31, 0xE3, 0x0D, 0xF8, 0x58, 0xC3, 0x83, 0x0F, 0x38, 0xC8, 0x86, 0x17, 0x03, 0xFC, 0x80,
0x0E, 0x17, 0x80, 0xFE, 0x80, 0x1E, 0x1F, 0xFC, 0x7F, 0xC0, 0x78, 0x1F, 0xFE, 0x3F, 0xFF, 0xF0,
0x3F, 0xFF, 0x1F, 0x4F, 0xF0, 0x7F, 0xFF, 0x9F, 0x49, 0xE0, 0xFF, 0xFF, 0xCF, 0x28, 0x01, 0xFF,
0xFF, 0xCF, 0x1F, 0x0F, 0xFF, 0xFF, 0xE7, 0x03, 0xFF, 0xFF, 0xFF, 0xE7, 0x01, 0xFF, 0xFF, 0xFF,
0xE7, 0x03, 0xFF, 0xFF, 0xFF, 0xE7, 0x05, 0xB7, 0xFE, 0xD7, 0xE7, 0x07, 0xFF, 0xFF, 0xFF, 0xE7,
0x07, 0xFF, 0xFF, 0xFE, 0x00, 0x07, 0xFE, 0x07, 0xFE, 0x00, 0x04, 0x92, 0x04, 0x92, 0x00, 0x02,
0xDA, 0x02, 0xD4, 0x00, 0x01, 0xB6, 0x01, 0xA8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

static const unsigned char PROGMEM Bowser_4[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x20, 0x13, 0x00, 0x00, 0x00, 0x50, 0x2A, 0x80, 0x00, 0x00, 0x51, 0x6E,
0x64, 0x00, 0x00, 0x92, 0x82, 0x5A, 0x00, 0x00, 0x9C, 0x02, 0x25, 0x00, 0x00, 0x9C, 0x7C, 0x6E,
0x80, 0x00, 0x9F, 0xFC, 0x61, 0x00, 0x01, 0xFF, 0xFE, 0xC2, 0x00, 0x02, 0xFF, 0xCF, 0xC2, 0x80,
0x04, 0x7F, 0x83, 0xE1, 0x40, 0x09, 0x7F, 0x07, 0xE0, 0x80, 0x07, 0x3F, 0x39, 0xF0, 0x80, 0x03,
0xBD, 0xD0, 0xF8, 0x80, 0x03, 0x59, 0xE0, 0xF0, 0x60, 0x03, 0xFE, 0x84, 0x7C, 0x78, 0x06, 0x1D,
0x0E, 0x72, 0x3A, 0x0C, 0x06, 0x3C, 0x61, 0xBF, 0x0B, 0x18, 0x70, 0x61, 0xE7, 0x19, 0x10, 0xC2,
0xE0, 0x7F, 0x10, 0x01, 0xC2, 0xF0, 0x1F, 0x10, 0x03, 0xC3, 0xFF, 0x8F, 0x18, 0x0F, 0x03, 0xFF,
0xC7, 0x1F, 0xFE, 0x07, 0xFF, 0xE3, 0x09, 0xFE, 0x0F, 0xFF, 0xF3, 0xFF, 0xFF, 0xFF, 0xC0, 0x00,
0xFF, 0xC0, 0xFF, 0xC0, 0x00, 0x92, 0x40, 0x92, 0x40, 0x00, 0x5B, 0x40, 0x5A, 0x80, 0x00, 0x36,
0xC0, 0x35, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

static const unsigned char PROGMEM Bowser_5[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x20, 0x13, 0x00, 0x00, 0x00, 0x50, 0x2A, 0x80, 0x00, 0x00, 0x51, 0x6E, 0x64, 0x00, 0x00,
0x92, 0x82, 0x5A, 0x00, 0x00, 0x9C, 0x02, 0x25, 0x00, 0x00, 0x9C, 0x7C, 0x6E, 0x80, 0x00, 0x9F,
0xFC, 0x61, 0x00, 0x01, 0xFF, 0xFE, 0xC2, 0x00, 0x02, 0xFF, 0xCF, 0xC2, 0x80, 0x04, 0x7F, 0x83,
0xE1, 0x40, 0x09, 0x7F, 0x07, 0xE0, 0x80, 0x07, 0x3F, 0x39, 0xF0, 0x80, 0x03, 0xBD, 0xD0, 0xF8,
0x80, 0x03, 0x59, 0xE0, 0xF0, 0x60, 0x03, 0xFE, 0x84, 0x7C, 0x78, 0x06, 0x1D, 0x0E, 0x72, 0x3A,
0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0xFF, 0xC0, 0xFF, 0xC0, 0x00, 0x92, 0x40, 0x92, 0x40, 0x00, 0x5B,
0x40, 0x5A, 0x80, 0x00, 0x36, 0xC0, 0x35, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

static const unsigned char PROGMEM Bowser_6[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x13, 0x00, 0x00, 0x00, 0x50, 0x2A,
0x80, 0x00, 0x00, 0x51, 0x6E, 0x64, 0x00, 0x00, 0x92, 0x82, 0x5A, 0x00, 0x00, 0x9C, 0x02, 0x27,
0x00, 0x00, 0x9C, 0x7C, 0x6E, 0x80, 0x00, 0x9F, 0xFC, 0x61, 0x00, 0x01, 0xFF, 0xFE, 0xC2, 0x00,
0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0xFF, 0xC0, 0xFF, 0xC0, 0x00, 0x92, 0x40, 0x92, 0x40, 0x00, 0x5B,
0x40, 0x5A, 0x80, 0x00, 0x36, 0xC0, 0x35, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const unsigned char PROGMEM CAT[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFB, 0x7F, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFB, 0x77, 0xFD, 0xFF, 0xFF, 0xFF, 0xFD, 0x6E, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0x6F, 0xBF, 0xFF, 0xEE, 0xBF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0xEF, 0x6D, 0xED, 0xFB, 0xFF, 0xFF, 0xFA, 0xED, 0xBB,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xFF, 0xFF, 0xF7, 0xFF, 0xAE, 0xFF, 0xFF, 0xFD, 0x3F, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xFD, 0xDF, 0xFE, 0xFB, 0xB5, 0xFF, 0xD6, 0xDB, 0xFF,
0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xEF, 0x5E, 0xFF, 0xFF, 0xFD, 0x7E, 0xDF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0xFF, 0xF6, 0xAF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xFF, 0xFF, 0x5F, 0xFF, 0x5F, 0xAD, 0xDF, 0xED, 0x7B, 0x7F,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF6, 0xF7, 0xFF, 0xF6, 0xAF, 0xFF,
0xFF, 0xFF, 0xFF, 0xDF, 0xFF, 0xFB, 0xDF, 0xFF, 0x7F, 0xFD, 0xFF, 0xBF, 0xFD, 0xAB, 0x3D, 0xDF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xBF, 0xFF, 0xD6, 0xF6, 0xBF, 0xF5, 0x5F, 0x7F,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xFF, 0x7F, 0xBF, 0xEB, 0x56, 0xB7, 0xFB,
0xFF, 0xFF, 0x77, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFD, 0xDB, 0xFF, 0xEA, 0xBD, 0xBF,
0xFF, 0xF5, 0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0x6E, 0xFE, 0xAA, 0xAF, 0xFF,
0xFF, 0xDF, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xF7, 0xBF, 0xDF, 0xA5, 0x5A, 0xDF,
0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x5E, 0xDA, 0xEB, 0xD5, 0x5F, 0xFF,
0xFF, 0xFD, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0x55, 0x6E, 0xDF,
0xFF, 0xFA, 0xBA, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xD5, 0xAB, 0xFA, 0xD2, 0xBB, 0xFF,
0xFF, 0xFA, 0xDF, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA, 0xBF, 0x7F, 0xEE, 0xA9, 0x5F, 0x77,
0xFF, 0xFA, 0xBF, 0x4F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7E, 0xC7, 0xDB, 0xFB, 0x54, 0xBB, 0xDF,
0xFF, 0xF9, 0x5E, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFC, 0x47, 0xFE, 0xAD, 0x2A, 0xFE, 0xFF,
0xFF, 0xFA, 0xBE, 0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x52, 0xAB, 0xFF, 0xFA, 0x95, 0x5B, 0xBF,
0xFF, 0x78, 0x9E, 0x97, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0x72, 0x47, 0xF7, 0xFF, 0xD1, 0xFE, 0xFF,
0xFF, 0xFD, 0x5A, 0x57, 0xEF, 0xFF, 0xFF, 0xFF, 0xD5, 0x3E, 0x13, 0xDF, 0xF5, 0x55, 0x77, 0xDF,
0xFD, 0xBF, 0x09, 0x2F, 0xFD, 0xFF, 0xFF, 0xFF, 0xF4, 0xBD, 0x49, 0xEB, 0xBF, 0xAA, 0xDD, 0x7F,
0xFF, 0xFE, 0x84, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xB2, 0x5C, 0x07, 0xDF, 0xEA, 0xD5, 0xFF, 0xFF,
0xFF, 0xEF, 0x91, 0x3F, 0xFB, 0xFF, 0xFF, 0xFD, 0xF9, 0x3D, 0x57, 0xD6, 0xFF, 0x69, 0x7B, 0x7F,
0xFF, 0xFF, 0xE4, 0xFF, 0xFF, 0x7F, 0xBF, 0xEF, 0x78, 0x58, 0x0F, 0x7B, 0xEF, 0x55, 0xEF, 0xFF,
0xFF, 0xBB, 0xFF, 0xFF, 0xFD, 0xDF, 0xFF, 0x75, 0xF9, 0x05, 0x1F, 0xAF, 0xF5, 0xD2, 0xFD, 0xBF,
0xFF, 0xED, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xBF, 0x7C, 0x20, 0x5F, 0xFF, 0xF7, 0x55, 0xBF, 0xFF,
0xFF, 0xFE, 0xB7, 0xFF, 0xFA, 0xB7, 0xFB, 0xED, 0xFE, 0x08, 0x7E, 0xAF, 0xFD, 0xAB, 0xF6, 0xFF,
0xFF, 0xFF, 0xDF, 0xFF, 0xFF, 0x7B, 0xEF, 0xFF, 0xFF, 0x85, 0xFF, 0xBF, 0xD6, 0xD5, 0xDF, 0xFF,
0xFF, 0xFE, 0xF5, 0x5F, 0xFD, 0xDF, 0xFA, 0xF5, 0xFF, 0xFF, 0xFA, 0xEF, 0x7B, 0x57, 0x7B, 0x7F,
0xFF, 0xFF, 0xBF, 0xEB, 0xFF, 0x6D, 0xEF, 0x7F, 0xFF, 0xFB, 0xEE, 0xBF, 0xF6, 0xAB, 0xFF, 0xFF,
0xFF, 0xFF, 0xF5, 0xBF, 0xFF, 0xF7, 0x7B, 0xBF, 0xDB, 0xFF, 0xB7, 0xFF, 0xED, 0xAE, 0xFD, 0xFF,
0xFF, 0xFF, 0xFF, 0x6F, 0xFD, 0xBE, 0xED, 0x57, 0xF5, 0x55, 0x6D, 0xEF, 0xFB, 0x57, 0xEF, 0xBF,
0xFF, 0xFF, 0xFB, 0xFB, 0xFF, 0xFB, 0xB6, 0xFF, 0xFB, 0xEB, 0xB7, 0x7F, 0xDD, 0xAD, 0xFE, 0xFF,
0xFF, 0xFF, 0xDE, 0xBF, 0xFF, 0xFF, 0xDB, 0x5F, 0xBA, 0xBD, 0x7F, 0xF7, 0x6A, 0x97, 0xB7, 0xFF,
0xFF, 0xFF, 0x7B, 0xEF, 0xDF, 0xFA, 0xA5, 0xFF, 0xFF, 0xD7, 0xFF, 0xFD, 0xFF, 0x5E, 0xFE, 0xFF,
0xFF, 0xFB, 0xFF, 0xBF, 0x7D, 0xFF, 0x52, 0xB7, 0xD5, 0x6D, 0xAF, 0xFF, 0x55, 0xAF, 0xFF, 0xFF,
0xFF, 0xFF, 0xEE, 0xFD, 0xFE, 0xD5, 0x52, 0xFE, 0xFF, 0xFE, 0xFE, 0xDD, 0xEE, 0xBB, 0xF7, 0xFF,
0xFF, 0xEF, 0x7F, 0xDF, 0xFF, 0x7E, 0xA8, 0x7F, 0xB5, 0x6B, 0xFB, 0xF7, 0x5A, 0xDF, 0xDE, 0xFF,
0xFF, 0xFF, 0xFD, 0xFF, 0xBD, 0xD5, 0x51, 0x5E, 0xFF, 0x7F, 0xEF, 0xFE, 0xF7, 0x5B, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0x7B, 0xFF, 0xF6, 0xA4, 0xBF, 0xDD, 0xB7, 0xBE, 0xFB, 0xBA, 0xFF, 0x7B, 0xFF,
0xFF, 0xFF, 0xEF, 0xFF, 0xFE, 0xB5, 0x2A, 0x7D, 0x6F, 0xFD, 0xFF, 0xDE, 0xFD, 0x5D, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFA, 0x81, 0x5F, 0xFF, 0xDF, 0xEF, 0xF5, 0xD6, 0xFF, 0xEE, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDE, 0xAA, 0x7F, 0x57, 0x77, 0x7D, 0x7F, 0xFE, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xB7, 0xBF, 0xEE, 0x55, 0x3F, 0x7D, 0xFF, 0xDF, 0xD5, 0xF5, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF5, 0x54, 0x9D, 0xBF, 0xDF, 0xFB, 0xFF, 0xDE, 0xDF, 0x7B, 0xFF,
0xFF, 0xFF, 0xFF, 0xFD, 0xBF, 0x5A, 0xEA, 0xBE, 0xDB, 0xFB, 0x7E, 0xD7, 0xF5, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xDF, 0xFE, 0xEF, 0xFD, 0x1F, 0x6E, 0xFD, 0xFF, 0xFF, 0xFB, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFE, 0xDF, 0x56, 0xF6, 0xAE, 0xB7, 0xBF, 0x6E, 0xDF, 0xAF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0xFF, 0x5F, 0xAB, 0x5A, 0xFF, 0xBF, 0x7F, 0xF7, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDD, 0xFF, 0xFF, 0xFF, 0xFF, 0x5D, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0xFF, 0xDF, 0xAB, 0x5F, 0xF6, 0xFF, 0xF7, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFF, 0xFF, 0xBF, 0xFD, 0x5F, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD5, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFF, 0xFF, 0xEF, 0xFF, 0xFF, 0xEF, 0xFD, 0x7F, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0xFF, 0xBF, 0xFB, 0x7F, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFF, 0xFF, 0xF7, 0xBF, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF
};

/****************************************************************************************
 * STATIC FUNCTION PROTOTYPES
 ***************************************************************************************/






 /****************************************************************************************
 * STATIC DEBUG AND TEST FUNCTIONS
 ****************************************************************************************/
static void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }    
  display.display();
}

static void testdrawcircle(void) {
  for (int16_t i=0; i<display.height(); i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
  }
}

static void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<display.height()/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
    display.display();
    color++;
  }
}

static void testdrawtriangle(void) {
  for (int16_t i=0; i<min(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    display.display();
  }
}

static void testfilltriangle(void) {
  uint8_t color = WHITE;
  for (int16_t i=min(display.width(),display.height())/2; i>0; i-=5) {
    display.fillTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

static void testdrawroundrect(void) {
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, WHITE);
    display.display();
  }
}

static void testfillroundrect(void) {
  uint8_t color = WHITE;
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}
   
static void testdrawrect(void) {
  for (int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
    display.display();
  }
}

static void testdrawline() {  
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, WHITE);
    display.display();
  }
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, WHITE);
    display.display();
  }
  delay(250);
  
  display.clearDisplay();
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
    display.display();
  }
  delay(250);
  
  display.clearDisplay();
  for (int16_t i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, WHITE);
    display.display();
  }
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE); 
    display.display();
  }
  delay(250);
}

static void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("scroll");
  display.display();
 
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);    
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}

 
/****************************************************************************************
 * STATIC FUNCTIONS
 ***************************************************************************************/
static bool display_driver_construct_bitmap_from_frame(const int* source_frame){
    bool ret = true;
    int  compressed_frame[128]; // has range of values from 64 to 0;
    int  tmp_byte = 0x00;

    for (int v=0; v< 128; v++){
        compressed_frame[v]= source_frame[v]/16;
    }

    for (int i=64; i>0; i--){          // loop over raws
        for (int j= 0; j<16; j++){     // loop over columns in groups of 8
            tmp_byte = 0x00;
            for (int x= 0; x<8; x++){
                if (compressed_frame[8*j+x]>=i){ //==
                    if (x==0){
                        tmp_byte |=0x80; 
                    } else if (x==1){
                        tmp_byte |=0x40;
                    } else if (x==2){
                        tmp_byte |=0x20;
                    } else if (x==3){
                        tmp_byte |=0x10;
                    } else if (x==4){
                        tmp_byte |=0x08;
                    } else if (x==5){
                        tmp_byte |=0x04;
                    } else if (x==6){
                        tmp_byte |=0x02;
                    } else if (x==7){
                        tmp_byte |=0x01;
                    }
                }
            }

            //store tmp in bitmap
            frame_bitmap[(64-i)*16+j]= tmp_byte;        
            
        }
    }


    return ret;
}

static void display_driver_test(){
    
    for (int i=0; i<100; i++){
        display.clearDisplay();
        display.drawBitmap(0, 0, Bowser_1,  BOWSERW,  BOWSERH, WHITE);
        display.display();
        delay(100);
            
        display.clearDisplay();
        display.drawBitmap(0, 0, Bowser_2,  BOWSERW,  BOWSERH, WHITE);
        display.display();
        delay(100);
            
        display.clearDisplay();
        display.drawBitmap(0, 0, Bowser_3,  BOWSERW,  BOWSERH, WHITE);
        display.display();
        delay(100);
            
        display.clearDisplay();
        display.drawBitmap(0, 0, Bowser_4,  BOWSERW,  BOWSERH, WHITE);
        display.display();
        delay(100);
            
        display.clearDisplay();
        display.drawBitmap(0, 0, Bowser_5,  BOWSERW,  BOWSERH, WHITE);
        display.display();
        delay(100);
            
        display.clearDisplay();
        display.drawBitmap(0, 0, Bowser_6,  BOWSERW,  BOWSERH, WHITE);
        display.display();
        delay(100);
            
    }
              
}
/****************************************************************************************
 * PUBLIC FUNCTIONS
 ***************************************************************************************/
bool display_driver_init(){
    bool ret = true;

    if(!display_driver_initialized){
        // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
        // init done
  
        // Show image buffer on the display hardware.
        // Since the buffer is intialized with an Adafruit splashscreen
        // internally, this will display the splashscreen.
        display.display();
        delay(2000);

        display_driver_initialized = true;
    }
    
    return ret;
}


bool display_driver_display_bitmap(uint8_t *bitmap){
    bool ret = true;

    if (!driver_in_testing_mode && bitmap == NULL){
        Serial.println("ERROR>> display_driver_display_bitmap: bitmap is NULL");
        return false;
    }
    
    display.clearDisplay();
    display.drawBitmap(0, 0, bitmap,  128,  64, WHITE);
    display.display();

    return ret;
}

void display_driver_display_frame(const int* frame_data){
    
    if (frame_data == NULL){
        Serial.println("ERROR>> display_driver_display_frame: frame_data is NULL");
        return;
    }

    // clear any existing data in the frame_bitmap
    memset(frame_bitmap, 0, 128);

    // construct bitmap from the provided frame data
    display_driver_construct_bitmap_from_frame(frame_data);

    //display the custruct bit map
    if (! display_driver_display_bitmap(frame_bitmap)){
        Serial.println("ERROR>> display_driver_display_frame: failed to display bit map");
    }
    
}

