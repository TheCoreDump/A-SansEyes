#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include "EyeBall.h"
#include "Eyes.h"
#include <Wire.h>


#define BUTTON_PIN 2


#define LEFT_EYE_ADDRESS 0x70
#define LEFT_EYE_ROTATION 3
#define LEFT_EYE_BRIGHTNESS 5

#define RIGHT_EYE_ADDRESS 0x72
#define RIGHT_EYE_ROTATION 3
#define RIGHT_EYE_BRIGHTNESS 5


EyesClass sansEyes = EyesClass();


void setup()
{
  //Serial.begin(9600);
  
	// Initialize the button
	pinMode(BUTTON_PIN, INPUT_PULLUP);

	// Setup the interrupt for the pin
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), OnButtonPressed, RISING);

	// Initialize the eyes.
	sansEyes.Init(LEFT_EYE_BRIGHTNESS, RIGHT_EYE_BRIGHTNESS, LEFT_EYE_ADDRESS, RIGHT_EYE_ADDRESS, LEFT_EYE_ROTATION, RIGHT_EYE_ROTATION);
}

void loop()
{
  sansEyes.Update();
}


void OnButtonPressed()
{
	sansEyes.StartBasicallyABadTime();
}
