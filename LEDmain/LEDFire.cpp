#include "LEDFire.h"
#include <Arduino.h>
#include "FastLED.h"

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
  rHTMLString = "<!DOCTYPE html>\n";
  rHTMLString += "<html>\n";
  rHTMLString += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
  rHTMLString += "<link rel=\"icon\" href=\"data:,\">\n";
  // CSS to style the on/off buttons 
  // Feel free to change the background-color and font-size attributes to fit your preferences
  rHTMLString += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  rHTMLString += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;\n";
  rHTMLString += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}\n";
  rHTMLString += ".button2 {background-color: #555555;}</style></head>\n";
  
  // Web Page Heading
  rHTMLString += "<body><h1>Loesch LED Fire</h1>\n";

  rHTMLString += "<p>[<span id='datetime'></span>]</p>\n";
  
  if (m_bActive)
  {
	rHTMLString += "<p>Current LED State: on</p>\n";
    rHTMLString += "<p><a href=\"/fire?action=off\"><button class=\"button button2\">OFF</button></a></p>\n";
  }
  else
  {
	rHTMLString += "<p>Current LED State: off</p>\n";
    rHTMLString += "<p><a href=\"/fire?action=on\"><button class=\"button\">ON</button></a></p>\n";
  }

  rHTMLString += "<script>\n";
  rHTMLString += "var dt = new Date()\n";
  rHTMLString += "document.getElementById(\"datetime\").innerHTML = ((\"0\"+dt.getDate()).slice(-2)) +\".\"+ ((\"0\"+(dt.getMonth()+1)).slice(-2)) +\".\"+ (dt.getFullYear()) +\" \"+ ((\"0\"+dt.getHours()).slice(-2)) +\":\"+ ((\"0\"+dt.getMinutes()).slice(-2))\n";
  rHTMLString += "</script>\n";

  rHTMLString += "<p></p>\n";
  rHTMLString += "<p></p>\n";
  rHTMLString += "<a href=\"/\">- HOME -</a>\n";
  rHTMLString += "</body></html>\n";
  
  // The HTTP response ends with another blank line
  rHTMLString += "\n";
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

void LEDFire::onEvent(std::vector<std::pair<String, String>>& rArguments)
{
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
