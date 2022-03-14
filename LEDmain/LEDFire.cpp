#include "LEDFire.h"
#include <Arduino.h>
#include "FastLED.h"
#include "GlobalDefs.h"

// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

bool gReverseDirection = false;

LEDFire::LEDFire(void)
{
	
}

LEDFire::~LEDFire(void)
{
	if (heat != nullptr)
	{
		delete[] heat;
	}
}

void LEDFire::CreateHTML(String& rHTMLString)
{
	CreateHTMLHeader(GetSubPage(), rHTMLString);
	CreateHTMLOnOffButtons(GetSubPageLink(), m_bActive, rHTMLString);
	CreateHTMLDateTimeFunction(rHTMLString);
	CreateHTMLFooter(rHTMLString);
}

void LEDFire::UpdateLEDs(CRGB* leds, int iNumLEDs)
{
	if (!m_bActive)
	{
		SetBlack(leds, iNumLEDs);
		return;
	}
	
	// Array of temperature readings at each simulation cell
	if (heat == nullptr)
	{
		heat = new uint8_t[iNumLEDs];
	}


  // Step 1.  Cool down every cell a little
	for( int i = 0; i < iNumLEDs; i++) {
	  heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / iNumLEDs) + 2));
	}
  
	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
	for( int k= iNumLEDs - 1; k >= 2; k--) {
	  heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
	}
	
	// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
	if( random8() < SPARKING ) {
	  int y = random8(7);
	  heat[y] = qadd8( heat[y], random8(160,255) );
	}

	// Step 4.  Map from heat cells to LED colors
	for( int j = 0; j < iNumLEDs; j++) {
	  CRGB color = HeatColor( heat[j]);
	  int pixelnumber;
	  if( gReverseDirection ) {
		pixelnumber = (iNumLEDs-1) - j;
	  } else {
		pixelnumber = j;
	  }
	  leds[pixelnumber] = color;
	}
}

String LEDFire::GetSubPage(void) const
{
	return "Fire";
}

String LEDFire::GetSubPageLink(void) const
{
	return "/fire";
}

std::unique_ptr<LEDConfigBase> LEDFire::createConfig(const String& rName)
{
	return nullptr; // no config.
}

void LEDFire::onEvent(std::vector<std::pair<String, String>>& rArguments)
{
	LEDBase::onEvent(rArguments);

	for (int i = 0; i < rArguments.size(); ++i)
	{
		if (rArguments[i].first == "action")
		{
			if (rArguments[i].second == "on")
			{
				SetActive(true);
			}
			else if (rArguments[i].second == "off")
			{
				SetActive(false);
			}
		}
	}
}
