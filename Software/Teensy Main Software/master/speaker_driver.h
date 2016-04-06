#ifndef SPEAKER_DRIVER_h
#define SPEAKER_DRIVER_h

#include "arduino.h"

/****************************************************************************************
 * DEFINES AND TYPE DEFINITIONS
 ****************************************************************************************/



/****************************************************************************************
 * GLOBAL VARIABLES
 ****************************************************************************************/
extern char* OBSTACLE_STOP_cstr;
extern char* DETECTING_WAIT_cstr;
extern char* FAILURE_SOUND_cstr;
extern char* GLITTER_cstr;
extern char* MINION_cstr;
extern char* MINION_cstr;

/****************************************************************************************
 * PUBLIC FUNCTIONS PROTOTYPES
 ****************************************************************************************/
bool speaker_driver_init();
bool speaker_driver_play_file(char* file_name);
bool speaker_driver_test();


#endif
