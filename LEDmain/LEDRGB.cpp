#include "LEDRGB.h"
#include <Arduino.h>
#include "FastLED.h"
#include <sstream>
#include "LEDConfigRGB.h"
#include "GlobalDefs.h"

LEDRGB::LEDRGB(void)
{
	m_configurations.push_back(make_unique<LEDConfigRGB>("default"));
	m_configurations.push_back(make_unique<LEDConfigRGB>("red", 255, 0, 0));
	m_configurations.push_back(make_unique<LEDConfigRGB>("green", 0, 255, 0));
	m_configurations.push_back(make_unique<LEDConfigRGB>("blue", 0, 0, 255));
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
	CreateHTMLConfigTableRow(m_configurations, GetSubPageLink(), getActiveConfig<LEDConfigRGB>().GetName(), rHTMLString);
	rHTMLString += "</table>\n";
	rHTMLString += "<p>Color Hex Value <span id=\"colHexValue\"></span></p>\n";
	rHTMLString += "\n";
  
	CreateHTMLRGBToHexFunction(rHTMLString);
	CreateHTMLUpdateValueFunction(m_URL, GetSubPageLink(), rHTMLString);
	CreateHTMLDateTimeFunction(rHTMLString);
	CreateHTMLAddConfigFunction(m_URL, GetSubPageLink(), rHTMLString);
	
	rHTMLString += "<script>\n";
	rHTMLString += "  document.getElementById(\"rgbColor\").value = rgbToHex(" + String(getActiveConfig<LEDConfigRGB>().m_uiRed) + "," + String(getActiveConfig<LEDConfigRGB>().m_uiGreen) + "," + String(getActiveConfig<LEDConfigRGB>().m_uiBlue) + ");\n";
	rHTMLString += "  document.getElementById(\"colHexValue\").innerHTML = rgbToHex(" + String(getActiveConfig<LEDConfigRGB>().m_uiRed) + "," + String(getActiveConfig<LEDConfigRGB>().m_uiGreen) + "," + String(getActiveConfig<LEDConfigRGB>().m_uiBlue) + ");\n";
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
	  leds[j] = CRGB(getActiveConfig<LEDConfigRGB>().m_uiRed, getActiveConfig<LEDConfigRGB>().m_uiGreen, getActiveConfig<LEDConfigRGB>().m_uiBlue);
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

std::unique_ptr<LEDConfigBase> LEDRGB::createConfig(const String& rName)
{
	auto ptr = make_unique<LEDConfigRGB>(rName);
	return ptr;
}

void LEDRGB::onEvent(std::vector<std::pair<String, String>>& rArguments)
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
		else if (rArguments[i].first == "red")
		{
			assertAndSetCustomConfig();
			getActiveConfig<LEDConfigRGB>().m_uiRed = (int8_t)rArguments[i].second.toInt();
		}
		else if (rArguments[i].first == "green")
		{
			assertAndSetCustomConfig();
			getActiveConfig<LEDConfigRGB>().m_uiGreen = (int8_t)rArguments[i].second.toInt();
		}
		else if (rArguments[i].first == "blue")
		{
			assertAndSetCustomConfig();
			getActiveConfig<LEDConfigRGB>().m_uiBlue = (int8_t)rArguments[i].second.toInt();
		}
		else if (rArguments[i].first == "rgbColor")
		{
			assertAndSetCustomConfig();
			int iHexAsInt;
			std::istringstream(rArguments[i].second.c_str()) >> std::hex >> iHexAsInt;

			getActiveConfig<LEDConfigRGB>().m_uiRed = ((iHexAsInt >> 16) & 0xFF);
			getActiveConfig<LEDConfigRGB>().m_uiGreen = ((iHexAsInt >> 8) & 0xFF);
			getActiveConfig<LEDConfigRGB>().m_uiBlue = ((iHexAsInt) & 0xFF);
			
			m_printFunc(String(getActiveConfig<LEDConfigRGB>().m_uiRed) + " - " + String(getActiveConfig<LEDConfigRGB>().m_uiGreen) + " - " + String(getActiveConfig<LEDConfigRGB>().m_uiBlue));
		}
	}
}
