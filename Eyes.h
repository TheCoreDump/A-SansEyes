// Eyes.h

#ifndef _EYES_h
#define _EYES_h

#include "EyeBall.h"
#include "AnimationData.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define BASICALLY_A_BAD_TIME_DURATION 10000

#define MIN_BLINK_TIME 3000
#define MAX_BLINK_TIME 7000

#define BLINK_FRAME_TIME 75
#define MOVEMENT_FRAME_TIME 20

#define EYE_MOVEMENT_FRAME_COUNT 8

#define MIN_STATE_CHANGE_TIME 10000
#define MAX_STATE_CHANGE_TIME 20000

class EyesClass
{
 protected:
	EyeBallClass leftEye;
	EyeBallClass rightEye;

	volatile EyeState currentState;
	volatile EyePosition previousPosition;
	volatile EyePosition currentPosition;
	volatile uint8_t currentFrame;

	uint64_t nextFrame;
	uint64_t nextBlink;
	uint64_t nextStateChange;

	uint64_t GetBasicallyABadTimeExpiration();
	uint64_t GetNextBlinkTime();
	uint64_t GetNextMovementTime();
	EyePosition GetRandomEyePosition();

	uint8_t GetLeftXForPosition(EyePosition position);
	uint8_t GetRightXForPosition(EyePosition position);
	uint8_t GetYForPosition(EyePosition position);
  
	void StartBlink();
	void UpdateBlink();
	void FinishBlink();

	void UpdateBasicallyABadTime();
	void FinishBasicallyABadTime();

	void StartEyeMoving(EyePosition newPosition);
	void UpdateEyeMoving();
	void FinishEyeMoving();

 public:
    EyesClass();

	void Init(uint8_t leftEyeBrightness, uint8_t rightEyeBrightness, uint8_t leftEyeAddress, uint8_t rightEyeAddress, uint8_t leftEyeRotation, uint8_t rightEyeRotation);
	void Reset();
	void StartBasicallyABadTime();
	void Update();
};

extern EyesClass Eyes;

#endif

