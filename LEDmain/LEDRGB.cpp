#include "LEDRGB.h"
#include <Arduino.h>
#include "FastLED.h"
#include <sstream>

LEDRGB::LEDRGB(void)
{
	
}

LEDRGB::~LEDRGB(void)
{
	
}

void LEDRGB::CreateHTML(String& rHTMLString)
{
	CreateHTMLHeader(GetSubPage(), rHTMLString);
	CreateHTMLOnOffButtons(GetSubPageLink(), m_bActive, rHTMLString);

	rHTMLString += "<table class =\"center\">\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>Color</td>\n";
	rHTMLString += "  <td><input type=\"color\" id=\"rgbColor\" name=\"rgbColor\" value=\"#ffffff\" onchange=\"updateVal(this.id, this.value);\"></td>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "</table>\n";
	rHTMLString += "<p>Color Hex Value <span id=\"colHexValue\"></span></p>\n";
	rHTMLString += "\n";
  
	CreateHTMLRGBToHexFunction(rHTMLString);
	CreateHTMLUpdateValueFunction(m_URL, GetSubPageLink(), rHTMLString);
	CreateHTMLDateTimeFunction(rHTMLString);
	
	rHTMLString += "<script>\n";
	rHTMLString += "  document.getElementById(\"rgbColor\").value = rgbToHex(" + String(m_iRed) + "," + String(m_iGreen) + "," + String(m_iBlue) + ");\n";
	rHTMLString += "  document.getElementById(\"colHexValue\").innerHTML = rgbToHex(" + String(m_iRed) + "," + String(m_iGreen) + "," + String(m_iBlue) + ");\n";
	rHTMLString += "</script>\n";
 
	CreateHTMLFooter(rHTMLString);
}

void LEDRGB::UpdateLEDs(CRGB* leds, int iNumLEDs)
{
	if (!m_bActive)
	{
		SetBlack(leds, iNumLEDs);
		return;
	}

	for(int j = 0; j < iNumLEDs; j++)
	{
	  leds[j] = CRGB(m_iRed, m_iGreen, m_iBlue);
	}
}

String LEDRGB::GetSubPage(void) const
{
	return "RGB simple";
}

String LEDRGB::GetSubPageLink(void) const
{
	return "/rgb";
}

void LEDRGB::onEvent(std::vector<std::pair<String, String>>& rArguments)
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
		else if (rArguments[i].first == "red")
		{
			m_iRed = (int8_t)rArguments[i].second.toInt();
		}
		else if (rArguments[i].first == "green")
		{
			m_iGreen = (int8_t)rArguments[i].second.toInt();
		}
		else if (rArguments[i].first == "blue")
		{
			m_iBlue = (int8_t)rArguments[i].second.toInt();
		}
		else if (rArguments[i].first == "rgbColor")
		{
			int iHexAsInt;
			std::istringstream(rArguments[i].second.c_str()) >> std::hex >> iHexAsInt;

			m_iRed = ((iHexAsInt >> 16) & 0xFF);
			m_iGreen = ((iHexAsInt >> 8) & 0xFF);
			m_iBlue = ((iHexAsInt) & 0xFF);
			
			m_printFunc(String(m_iRed) + " - " + String(m_iGreen) + " - " + String(m_iBlue));
		}
	}
}
