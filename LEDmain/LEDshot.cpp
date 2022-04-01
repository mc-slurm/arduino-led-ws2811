#include "LEDShot.h"
#include <Arduino.h>
#include "FastLED.h"
#include "LEDConfigShot.h"
#include "GlobalDefs.h"
#include "Logger.h"
#include <sstream>

LEDShot::LEDShot(void)
{
	m_configurations.push_back(make_unique<LEDConfigShot>("default"));
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
	rHTMLString += "<td><input type=\"range\" id=\"speed\" min=\"1\" max=\"10\" step=\"1\" value=\"" + String(getActiveConfig<LEDConfigShot>().m_uiSpeed) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
	rHTMLString += "    <label for=\"speed\">" + String(getActiveConfig<LEDConfigShot>().m_uiSpeed) + "</label>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>Size</td>\n";
	rHTMLString += "<td><input type=\"range\" id=\"shotsize\" min=\"1\" max=\"20\" step=\"1\" value=\"" + String(getActiveConfig<LEDConfigShot>().m_uiSize) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
	rHTMLString += "    <label for=\"shotsize\">" + String(getActiveConfig<LEDConfigShot>().m_uiSize) + "</label>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>Color</td>\n";
	rHTMLString += "  <td><input type=\"color\" id=\"rgbColor\" name=\"rgbColor\" value=\"#ffffff\" onchange=\"updateVal(this.id, this.value);\"></td>\n";
	rHTMLString += "</tr>\n";
	CreateHTMLConfigTableRow(m_configurations, GetSubPageLink(), getActiveConfig<LEDConfigShot>().GetName(), rHTMLString);
	rHTMLString += "</table>\n";

	rHTMLString += "<p><a href=\"/shot?action=shot\"><button class=\"button button3\">Fire</button></a></p>\n";

	rHTMLString += "<script>\n";
	rHTMLString += "  document.getElementById(\"rgbColor\").value = rgbToHex(" + String(getActiveConfig<LEDConfigShot>().m_uiRed) + "," + String(getActiveConfig<LEDConfigShot>().m_uiGreen) + "," + String(getActiveConfig<LEDConfigShot>().m_uiBlue) + ");\n";
	rHTMLString += "</script>\n";

	CreateHTMLRGBToHexFunction(rHTMLString);
	CreateHTMLUpdateValueFunction(m_URL, GetSubPageLink(), rHTMLString);
	CreateHTMLDateTimeFunction(rHTMLString);
	CreateHTMLAddConfigFunction(m_URL, GetSubPageLink(), rHTMLString);
	
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

	for(int j = iNumLEDs-getActiveConfig<LEDConfigShot>().m_uiSpeed-1; j >= 0; --j)
	{
		leds[j+getActiveConfig<LEDConfigShot>().m_uiSpeed] = leds[j];
	}
	
	if (m_uiShotPosition < getActiveConfig<LEDConfigShot>().m_uiSize-1)
	{
		float fRed = ((float)(getActiveConfig<LEDConfigShot>().m_uiSize - m_uiShotPosition) / (float)getActiveConfig<LEDConfigShot>().m_uiSize) * (float)getActiveConfig<LEDConfigShot>().m_uiRed;
		float fGreen = ((float)(getActiveConfig<LEDConfigShot>().m_uiSize - m_uiShotPosition) / (float)getActiveConfig<LEDConfigShot>().m_uiSize) * (float)getActiveConfig<LEDConfigShot>().m_uiGreen;
		float fBlue = ((float)(getActiveConfig<LEDConfigShot>().m_uiSize - m_uiShotPosition) / (float)getActiveConfig<LEDConfigShot>().m_uiSize) * (float)getActiveConfig<LEDConfigShot>().m_uiBlue;
		leds[0] =  CRGB((uint8_t)fRed, (uint8_t)fGreen, (uint8_t)fBlue);

		//LED_LOG("Pos: " + String(m_uiShotPosition) + " Color: " + String((uint8_t)fRed) + " - " + String((uint8_t)fGreen) + " - " + String((uint8_t)fBlue));
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

std::unique_ptr<LEDConfigBase> LEDShot::createConfig(const String& rName)
{
	auto ptr = make_unique<LEDConfigShot>(rName);
	return ptr;
}

void LEDShot::onEvent(std::vector<std::pair<String, String>>& rArguments)
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
			else if (rArguments[i].second == "shot")
			{
				m_uiShotPosition = 0;
				LED_LOG("shot!");
			}
		}
		else if (rArguments[i].first == "speed")
		{
			assertAndSetCustomConfig();
			getActiveConfig<LEDConfigShot>().m_uiSpeed = (uint8_t)rArguments[i].second.toInt();
		}
		else if (rArguments[i].first == "shotsize")
		{
			assertAndSetCustomConfig();
			getActiveConfig<LEDConfigShot>().m_uiSize = (uint8_t)rArguments[i].second.toInt();
		}
		else if (rArguments[i].first == "rgbColor")
		{
			assertAndSetCustomConfig();
			int iHexAsInt;
			std::istringstream(rArguments[i].second.c_str()) >> std::hex >> iHexAsInt;

			getActiveConfig<LEDConfigShot>().m_uiRed = ((iHexAsInt >> 16) & 0xFF);
			getActiveConfig<LEDConfigShot>().m_uiGreen = ((iHexAsInt >> 8) & 0xFF);
			getActiveConfig<LEDConfigShot>().m_uiBlue = ((iHexAsInt) & 0xFF);

			LED_LOG("Color: " + String(getActiveConfig<LEDConfigShot>().m_uiRed) + " - " + String(getActiveConfig<LEDConfigShot>().m_uiGreen) + " - " + String(getActiveConfig<LEDConfigShot>().m_uiBlue));
		}
	}
}
