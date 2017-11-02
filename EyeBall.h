// EyeBall.h

#ifndef _EYEBALL_h
#define _EYEBALL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "AnimationData.h"
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

//#define RENDER_PUPIL(x, y) EyeMatrix.fillCircle(x, y, 1, LED_ON);
#define RENDER_PUPIL(x, y) EyeMatrix.fillRect(x - 1, y - 1, 3, 3, LED_ON);

enum EyeOpenState { FullyOpen, HalfClosed, FullyClosed };

class EyeBallClass
{
protected:
	// Protected members
	Adafruit_8x8matrix EyeMatrix = Adafruit_8x8matrix();

	volatile uint8_t currentX;
	volatile uint8_t currentY;
	volatile EyeOpenState eyeOpenState;


public:
	EyeBallClass();

	void Close();
	void Init(uint8_t eyeBrightness, uint8_t eyeAddress, uint8_t eyeRotation);
	void DrawPupilAt(uint16_t x, uint16_t y, EyeOpenState newEyeOpenState);
	void StartBasicallyABadTime();

};

extern EyeBallClass EyeBall;

#endif

