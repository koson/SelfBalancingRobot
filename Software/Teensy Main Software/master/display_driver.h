#ifndef DISPLAY_DRIVER_h
#define DISPLAY_DRIVER_h

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

/****************************************************************************************
 * DEFINES AND TYPE DEFINITIONS
 ****************************************************************************************/
 

/****************************************************************************************
 * PUBLIC FUNCTIONS PROTOTYPES
 ****************************************************************************************/
bool display_driver_init();
bool display_driver_display_bitmap(uint8_t *bitmap);
void display_driver_display_frame(int* frame_data);


#endif


