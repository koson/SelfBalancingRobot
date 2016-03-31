#ifndef LED_DRIVER_h
#define LED_DRIVER_h

#include "arduino.h"

/****************************************************************************************
 * DEFINES AND TYPE DEFINITIONS
 ****************************************************************************************/
#define MAX_DISCARDED_LED_UPDATES 100
#define LED_ROW_LENGTH            9
#define LED_FRAME_COUNT           13

/****************************************************************************************
 * PUBLIC FUNCTIONS PROTOTYPES
 ****************************************************************************************/
bool led_driver_init();
bool led_driver_continue_talking();


#endif

