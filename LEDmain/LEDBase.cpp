#include "LEDBase.h"
#include <Arduino.h>
#include "FastLED.h"

void LEDBase::SetBlack(CRGB* leds, int iNumLEDs)
{
	for(int j = 0; j < iNumLEDs; j++)
	{
		leds[j] = CRGB::Black;
	}
}
