#include "LEDShot.h"
#include <Arduino.h>
#include "FastLED.h"
#include <sstream>

LEDShot::LEDShot(void)
{
	
}

LEDShot::~LEDShot(void)
{
	
}

void LEDShot::CreateHTML(String& rHTMLString)
{
	CreateHTMLHeader(GetSubPage(), rHTMLString);
	CreateHTMLOnOffButtons(GetSubPageLink(), m_bActive, rHTMLString);
  
	rHTMLString += "<table class =\"center\">\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <th>Variable</th>\n";
	rHTMLString += "  <th>Value</th>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>Speed</td>\n";
	rHTMLString += "<td><input type=\"range\" id=\"speed\" min=\"1\" max=\"10\" step=\"1\" value=\"" + String(m_uiSpeed) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
	rHTMLString += "    <label for=\"speed\">" + String(m_uiSpeed) + "</label>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>Size</td>\n";
	rHTMLString += "<td><input type=\"range\" id=\"shotsize\" min=\"1\" max=\"20\" step=\"1\" value=\"" + String(m_uiSize) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
	rHTMLString += "    <label for=\"shotsize\">" + String(m_uiSize) + "</label>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>Color</td>\n";
	rHTMLString += "  <td><input type=\"color\" id=\"rgbColor\" name=\"rgbColor\" value=\"#ffffff\" onchange=\"updateVal(this.id, this.value);\"></td>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "</table>\n";

	rHTMLString += "<p><a href=\"/shot?action=shot\"><button class=\"button button3\">Fire</button></a></p>\n";

	rHTMLString += "<script>\n";
	rHTMLString += "  document.getElementById(\"rgbColor\").value = rgbToHex(" + String(m_iRed) + "," + String(m_iGreen) + "," + String(m_iBlue) + ");\n";
	rHTMLString += "</script>\n";

	CreateHTMLRGBToHexFunction(rHTMLString);
	CreateHTMLUpdateValueFunction(m_URL, GetSubPageLink(), rHTMLString);
	CreateHTMLDateTimeFunction(rHTMLString);
	
	CreateHTMLFooter(rHTMLString);
}

void LEDShot::UpdateLEDs(CRGB* leds, int iNumLEDs)
{
	if (!m_bActive)
	{
		SetBlack(leds, iNumLEDs);
		return;
	}
	
	// if (m_uiCurTime < m_uiTime)
	// {
		// m_uiCurTime += m_uiSpeed;
		// return;
	// }
	// m_uiCurTime = 0;

	for(int j = iNumLEDs-m_uiSpeed-1; j >= 0; --j)
	{
		leds[j+m_uiSpeed] = leds[j];
	}
	
	if (m_uiShotPosition < m_uiSize-1)
	{
		float fRed = ((float)(m_uiSize - m_uiShotPosition) / (float)m_uiSize) * (float)m_iRed;
		float fGreen = ((float)(m_uiSize - m_uiShotPosition) / (float)m_uiSize) * (float)m_iGreen;
		float fBlue = ((float)(m_uiSize - m_uiShotPosition) / (float)m_uiSize) * (float)m_iBlue;
		leds[0] =  CRGB((uint8_t)fRed, (uint8_t)fGreen, (uint8_t)fBlue);

		//m_printFunc("Pos: " + String(m_uiShotPosition) + " Color: " + String((uint8_t)fRed) + " - " + String((uint8_t)fGreen) + " - " + String((uint8_t)fBlue));
		++m_uiShotPosition;
	}
	else
	{
		leds[0] = CRGB::Black;
	}
}

String LEDShot::GetSubPage(void) const
{
	return "Shot";
}

String LEDShot::GetSubPageLink(void) const
{
	return "/shot";
}

void LEDShot::onEvent(std::vector<std::pair<String, String>>& rArguments)
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
			else if (rArguments[i].second == "shot")
			{
				m_uiShotPosition = 0;
				m_printFunc("shot!");
			}
		}
		else if (rArguments[i].first == "speed")
		{
			m_uiSpeed = (uint8_t)rArguments[i].second.toInt();
		}
		else if (rArguments[i].first == "shotsize")
		{
			m_uiSize = (uint8_t)rArguments[i].second.toInt();
		}
		else if (rArguments[i].first == "rgbColor")
		{
			int iHexAsInt;
			std::istringstream(rArguments[i].second.c_str()) >> std::hex >> iHexAsInt;

			m_iRed = ((iHexAsInt >> 16) & 0xFF);
			m_iGreen = ((iHexAsInt >> 8) & 0xFF);
			m_iBlue = ((iHexAsInt) & 0xFF);

			m_printFunc("Color: " + String(m_iRed) + " - " + String(m_iGreen) + " - " + String(m_iBlue));
		}
	}
}
