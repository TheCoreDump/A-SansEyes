#ifndef _ANIMATIONDATA_h
#define _ANIMATIONDATA_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

enum Eye { LeftEye, RightEye };

enum Direction { Forward, Backward };

enum EyeState { BasicallyABadTime, Blinking, Moving, Static };

enum EyePosition { Center, Left, Right, Up, Down };


static const uint8_t PROGMEM
	bmp_bascially_bad_time[] =
		{ B00111100,
		  B01111110,
		  B11000011,
		  B11000011,
		  B11000011,
		  B11000011,
		  B01111110,
		  B00111100 };


#endif
