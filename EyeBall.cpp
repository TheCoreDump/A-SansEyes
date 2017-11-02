#include "EyeBall.h"

EyeBallClass::EyeBallClass()
{
}


void EyeBallClass::Init(uint8_t eyeBrightness, uint8_t eyeAddress, uint8_t eyeRotation)
{
	EyeMatrix.begin(eyeAddress);
	EyeMatrix.setRotation(eyeRotation);
	EyeMatrix.setBrightness(eyeBrightness);

	EyeMatrix.clear();
	EyeMatrix.writeDisplay();
}


void EyeBallClass::Close()
{
	EyeMatrix.clear();
	EyeMatrix.writeDisplay();
}


void EyeBallClass::DrawPupilAt(uint16_t x, uint16_t y, EyeOpenState newEyeOpenState)
{
	// Stash the current state of the eye
	// Not needed anywhere right now, but might be nice to have.
	currentX = x;
	currentY = y;
	eyeOpenState = newEyeOpenState;


	// Clear the current state of the matrix
	EyeMatrix.clear();

	if (eyeOpenState == HalfClosed)
	{
		// This is the case where we are mid-blink
		EyeMatrix.drawLine(currentX - 1, currentY, currentX + 1, currentY, LED_ON);
	}
	else if (eyeOpenState == FullyOpen)
	{
		// This is the case where the eye is wide open
		RENDER_PUPIL(currentX, currentY);
	}

	// Push the results to the matrix
	EyeMatrix.writeDisplay();
}


void EyeBallClass::StartBasicallyABadTime()
{
	EyeMatrix.clear();
	EyeMatrix.drawBitmap(0, 0, bmp_bascially_bad_time, 8, 8, LED_ON);
	EyeMatrix.writeDisplay();
}


EyeBallClass EyeBall;

