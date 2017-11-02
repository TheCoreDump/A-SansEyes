#include "Eyes.h"


/*

  Constructor

*/

EyesClass::EyesClass()
{
}


/*

  Initialization Function

*/

void EyesClass::Init(uint8_t leftEyeBrightness, uint8_t rightEyeBrightness, uint8_t leftEyeAddress, uint8_t rightEyeAddress, uint8_t leftEyeRotation, uint8_t rightEyeRotation)
{
	leftEye.Init(leftEyeBrightness, leftEyeAddress, leftEyeRotation);
	rightEye.Init(rightEyeBrightness, rightEyeAddress, rightEyeRotation);

	Reset();
}


/*

  Functions to get the various durations used by the eyes

*/
uint64_t EyesClass::GetBasicallyABadTimeExpiration()
{
	return millis() + BASICALLY_A_BAD_TIME_DURATION;
}

uint64_t EyesClass::GetNextBlinkTime()
{
	return millis() + random(MIN_BLINK_TIME, MAX_BLINK_TIME);
}

uint64_t EyesClass::GetNextMovementTime()
{
	return millis() + random(MIN_STATE_CHANGE_TIME, MAX_STATE_CHANGE_TIME);
}



/* 

  Functions to calculate the positions of the eyes

*/

uint8_t EyesClass::GetLeftXForPosition(EyePosition position)
{
	// Set the X
	switch (position)
	{
		case Left:
			return 1;
		case Right:
			return 6;
		case Center:
		case Up:
		case Down:
			return 3;
	}

	return 0;
}

uint8_t EyesClass::GetRightXForPosition(EyePosition position)
{
	// Set the X
	switch (position)
	{
		case Left:
			return 1;
		case Right:
			return 6;
		case Center:
		case Up:
		case Down:
			return 4;
	}

	return 0;
}

uint8_t EyesClass::GetYForPosition(EyePosition position)
{
	// Set the Y
	switch (position)
	{
		case Center:
		case Left:
		case Right:
			return 4;
		case Up:
			return 1;
		case Down:
			return 6;
	}

	return 1;
}


/*

   Blink functions

*/


void EyesClass::StartBlink()
{
	currentFrame = 0;
	currentState = Blinking;
	nextFrame = BLINK_FRAME_TIME + millis();
}


void EyesClass::UpdateBlink()
{
	if (nextFrame < millis())
	{
		// If our current frame count is 3, then we are done.
		if (currentFrame == 3)
		{
			FinishBlink();
			return;
		}


		switch (currentFrame)
		{
			case 0:
			case 2:
				leftEye.DrawPupilAt(GetLeftXForPosition(currentPosition), GetYForPosition(currentPosition), HalfClosed);
				rightEye.DrawPupilAt(GetRightXForPosition(currentPosition), GetYForPosition(currentPosition), HalfClosed);
				break;
			case 1:
				leftEye.DrawPupilAt(GetLeftXForPosition(currentPosition), GetYForPosition(currentPosition), FullyClosed);
				rightEye.DrawPupilAt(GetRightXForPosition(currentPosition), GetYForPosition(currentPosition), FullyClosed);
				break;
		}

		// Increment the frame counter, and set the expiration time for the current frame.
		currentFrame++;
		nextFrame = BLINK_FRAME_TIME + millis();
	}
}

void EyesClass::FinishBlink()
{
	currentState = Static;
	currentFrame = 0;

	leftEye.DrawPupilAt(GetLeftXForPosition(currentPosition), GetYForPosition(currentPosition), FullyOpen);
	rightEye.DrawPupilAt(GetRightXForPosition(currentPosition), GetYForPosition(currentPosition), FullyOpen);

	nextBlink = GetNextBlinkTime();
}


/*

  Basically A Bad Time Functions

*/

void EyesClass::StartBasicallyABadTime()
{
	// Set the current state
	currentState = BasicallyABadTime;
	currentPosition = Center;

	// Set the expiration time
	nextStateChange = GetBasicallyABadTimeExpiration();
}


void EyesClass::UpdateBasicallyABadTime()
{
	// See if we have expired
	if (nextStateChange < millis()) {
		FinishBasicallyABadTime();
		return;
	}

	// If not, re-render the eyes
	leftEye.StartBasicallyABadTime();
	rightEye.Close();
}

void EyesClass::FinishBasicallyABadTime()
{
	Reset();
}


/*

  Eye moving functions

*/


void EyesClass::StartEyeMoving(EyePosition newPosition)
{
	if (currentPosition != newPosition)
	{
		previousPosition = currentPosition;
		currentPosition = newPosition;

		currentState = Moving;
		currentFrame = 0;

    nextFrame = millis() + MOVEMENT_FRAME_TIME;
	}
}

void EyesClass::UpdateEyeMoving()
{
	if (nextFrame < millis())
	{
		uint8_t prevLeftX, prevRightX, prevY;
		uint8_t destLeftX, destRightX, destY;
		uint8_t newLeftX, newRightX, newY;

		prevLeftX = GetLeftXForPosition(previousPosition);
		prevRightX = GetRightXForPosition(previousPosition);
		prevY = GetYForPosition(previousPosition);

		destLeftX = GetLeftXForPosition(currentPosition);
		destRightX = GetRightXForPosition(currentPosition);
		destY = GetYForPosition(currentPosition);

		newLeftX = (((destLeftX - prevLeftX) * currentFrame) / EYE_MOVEMENT_FRAME_COUNT) + prevLeftX;
		newRightX = (((destRightX - prevRightX) * currentFrame) / EYE_MOVEMENT_FRAME_COUNT) + prevRightX;
		newY = (((destY - prevY) * currentFrame) / EYE_MOVEMENT_FRAME_COUNT) + prevY;

		leftEye.DrawPupilAt(newLeftX, newY, FullyOpen);
		rightEye.DrawPupilAt(newRightX, newY, FullyOpen);

		if (currentFrame >= EYE_MOVEMENT_FRAME_COUNT)
		{
			FinishEyeMoving();
			return;
		}
		else
		{
			currentFrame++;
			nextFrame = millis() + MOVEMENT_FRAME_TIME;
		}
	}
}

void EyesClass::FinishEyeMoving()
{
	currentState = Static;
	currentFrame = 0;
	nextStateChange = GetNextMovementTime();

	leftEye.DrawPupilAt(GetLeftXForPosition(currentPosition), GetYForPosition(currentPosition), FullyOpen);
	rightEye.DrawPupilAt(GetRightXForPosition(currentPosition), GetYForPosition(currentPosition), FullyOpen);
}


EyePosition EyesClass::GetRandomEyePosition()
{
	uint8_t newPosition = random(0, 4);

	switch (newPosition)
	{
		case 1:
			return Left;
		case 2:
			return Right;
		case 3:
			return Up;
		case 4:
			return Down;
		default:
			return Center;
	}
}




/*

  Reset function takes it back to looking straight ahead, and sets the nextBlink, and nextStateChange timer values.

*/

void EyesClass::Reset()
{
	currentState = Static;
	currentPosition = previousPosition = Center;
	currentFrame = 0;

	nextStateChange = GetNextMovementTime();
	nextBlink = GetNextBlinkTime();

	leftEye.DrawPupilAt(GetLeftXForPosition(currentPosition), GetYForPosition(currentPosition), FullyOpen);
	rightEye.DrawPupilAt(GetRightXForPosition(currentPosition), GetYForPosition(currentPosition), FullyOpen);
}



/*

  Update function - used to determine the current state, render that state, and set the next state as appropriate.

*/

void EyesClass::Update()
{
	// Check to see if we are in basically a bad time.
	if (currentState == BasicallyABadTime)
	{
		UpdateBasicallyABadTime();
		return;
	} 

	// Check to see if we are blinking
	if (currentState == Blinking)
	{
		UpdateBlink();
		return;
	} 

	// Check to see if the eyes are moving
	if (currentState == Moving)
	{
		UpdateEyeMoving();
		return;
	} 

	// At this point, the eyes are just static.  
	if (currentState == Static)
	{
		leftEye.DrawPupilAt(GetLeftXForPosition(currentPosition), GetYForPosition(currentPosition), FullyOpen);
		rightEye.DrawPupilAt(GetRightXForPosition(currentPosition), GetYForPosition(currentPosition), FullyOpen);
	}


	// Now we check to see if any of the timers have expired
	if (nextBlink < millis())
	{
		StartBlink();
		return;
	}


	// Check to see if we need to look somewhere else
	if (nextStateChange < millis())
	{
		StartEyeMoving(GetRandomEyePosition());
		return;
	}
}


EyesClass Eyes;

