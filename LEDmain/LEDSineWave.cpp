#include "LEDSineWave.h"
#include <Arduino.h>
#include "FastLED.h"
#include "LEDConfigSineWave.h"
#include "GlobalDefs.h"
#include <sstream>

LEDSineWave::LEDSineWave(void)
{
	m_configurations.push_back(make_unique<LEDConfigSineWave>("default"));
}

LEDSineWave::~LEDSineWave(void)
{
	
}

void LEDSineWave::CreateHTML(String& rHTMLString)
{
	CreateHTMLHeader(GetSubPage(), rHTMLString);
	CreateHTMLOnOffButtons(GetSubPageLink(), m_bActive, rHTMLString);
  
	rHTMLString += "<p>A * sin(kx-wt+o)+D</p>\n";

	rHTMLString += "<table class =\"center\">\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <th>Variable</th>\n";
	rHTMLString += "  <th>Value</th>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>num waves</td>\n";
	rHTMLString += "<td><input type=\"range\" id=\"k\" min=\"0\" max=\"1\" step=\"0.01\" value=\"" + String(getActiveConfig<LEDConfigSineWave>().m_fk) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
	rHTMLString += "    <label for=\"k\">" + String(getActiveConfig<LEDConfigSineWave>().m_fk) + "</label>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>wave speed</td>\n";
	rHTMLString += "<td><input type=\"range\" id=\"w\" min=\"0\" max=\"5\" step=\"0.1\" value=\"" + String(getActiveConfig<LEDConfigSineWave>().m_fw) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
	rHTMLString += "    <label for=\"w\">" + String(getActiveConfig<LEDConfigSineWave>().m_fw) + "</label>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>wave shift</td>\n";
	rHTMLString += "<td><input type=\"range\" id=\"o\" min=\"0\" max=\"3\" step=\"0.1\" value=\"" + String(getActiveConfig<LEDConfigSineWave>().m_fo) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
	rHTMLString += "    <label for=\"o\">" + String(getActiveConfig<LEDConfigSineWave>().m_fo) + "</label>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>time step</td>\n";
	rHTMLString += "<td><input type=\"range\" id=\"dt\" min=\"0\" max=\"1\" step=\"0.001\" value=\"" + String(getActiveConfig<LEDConfigSineWave>().m_fdT) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
	rHTMLString += "    <label for=\"dt\">" + String(getActiveConfig<LEDConfigSineWave>().m_fdT) + "</label>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>Color high</td>\n";
	rHTMLString += "  <td><input type=\"color\" id=\"rgbColorHigh\" name=\"rgbColorHigh\" value=\"#ffffff\" onchange=\"updateVal(this.id, this.value);\"></td>\n";
	rHTMLString += "</tr>\n";  
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>Color low</td>\n";
	rHTMLString += "  <td><input type=\"color\" id=\"rgbColorLow\" name=\"rgbColorLow\" value=\"#ffffff\" onchange=\"updateVal(this.id, this.value);\"></td>\n";
	rHTMLString += "</tr>\n";
	CreateHTMLConfigTableRow(m_configurations, GetSubPageLink(), getActiveConfig<LEDConfigSineWave>().GetName(), rHTMLString);
	rHTMLString += "</table>\n";
  
	CreateHTMLRGBToHexFunction(rHTMLString);
	CreateHTMLUpdateValueFunction(m_URL, GetSubPageLink(), rHTMLString);
	CreateHTMLDateTimeFunction(rHTMLString);
	CreateHTMLAddConfigFunction(m_URL, GetSubPageLink(), rHTMLString);
  
	rHTMLString += "<script>\n";
	rHTMLString += "  document.getElementById(\"rgbColorHigh\").value = rgbToHex(" + String(getActiveConfig<LEDConfigSineWave>().m_uiRedHigh) + "," + String(getActiveConfig<LEDConfigSineWave>().m_uiGreenHigh) + "," + String(getActiveConfig<LEDConfigSineWave>().m_uiBlueHigh) + ");\n";
	rHTMLString += "  document.getElementById(\"rgbColorLow\").value = rgbToHex(" + String(getActiveConfig<LEDConfigSineWave>().m_uiRedLow) + "," + String(getActiveConfig<LEDConfigSineWave>().m_uiGreenLow) + "," + String(getActiveConfig<LEDConfigSineWave>().m_uiBlueLow) + ");\n";
	rHTMLString += "</script>\n";

	CreateHTMLFooter(rHTMLString);
}

void LEDSineWave::UpdateLEDs(CRGB* leds, int iNumLEDs)
{
	if (!m_bActive)
	{
		m_fTime = 0.f;
		SetBlack(leds, iNumLEDs);
		return;
	}

	for(int j = 0; j < iNumLEDs; j++)
	{
		// scale to [-0.5 .. 0.5] and shift by 0.5 to get [0 .. 1]
		float fVal = sinf(getActiveConfig<LEDConfigSineWave>().m_fk * (float)j - getActiveConfig<LEDConfigSineWave>().m_fw * m_fTime + getActiveConfig<LEDConfigSineWave>().m_fo)/2.f + 0.5f;

		float fRed = fVal * (float)getActiveConfig<LEDConfigSineWave>().m_uiRedHigh + (1.f - fVal) * (float)getActiveConfig<LEDConfigSineWave>().m_uiRedLow;
		float fGreen = fVal * (float)getActiveConfig<LEDConfigSineWave>().m_uiGreenHigh + (1.f - fVal) * (float)getActiveConfig<LEDConfigSineWave>().m_uiGreenLow;
		float fBlue = fVal * (float)getActiveConfig<LEDConfigSineWave>().m_uiBlueHigh + (1.f - fVal) * (float)getActiveConfig<LEDConfigSineWave>().m_uiBlueLow;

		leds[j] =  CRGB((uint8_t)fRed, (uint8_t)fGreen, (uint8_t)fBlue);
//		leds[j] =  CRGB(120, (uint8_t)((float)60 * fVal), 0);
	}
	m_fTime += getActiveConfig<LEDConfigSineWave>().m_fdT;
}

String LEDSineWave::GetSubPage(void) const
{
	return "Sine Wave";
}

String LEDSineWave::GetSubPageLink(void) const
{
	return "/sinewave";
}

std::unique_ptr<LEDConfigBase> LEDSineWave::createConfig(const String& rName)
{
	auto ptr = make_unique<LEDConfigSineWave>(rName);
	return ptr;
}

void LEDSineWave::onEvent(std::vector<std::pair<String, String>>& rArguments)
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
		else if (rArguments[i].first == "k")
		{
			assertAndSetCustomConfig();
			getActiveConfig<LEDConfigSineWave>().m_fk = rArguments[i].second.toFloat();
		}
		else if (rArguments[i].first == "w")
		{
			assertAndSetCustomConfig();
			getActiveConfig<LEDConfigSineWave>().m_fw = rArguments[i].second.toFloat();
		}
		else if (rArguments[i].first == "o")
		{
			assertAndSetCustomConfig();
			getActiveConfig<LEDConfigSineWave>().m_fo = rArguments[i].second.toFloat();
		}
		else if (rArguments[i].first == "dt")
		{
			getActiveConfig<LEDConfigSineWave>().m_fdT = rArguments[i].second.toFloat();
		}
		else if (rArguments[i].first == "rgbColorHigh")
		{
			assertAndSetCustomConfig();
			int iHexAsInt;
			std::istringstream(rArguments[i].second.c_str()) >> std::hex >> iHexAsInt;

			getActiveConfig<LEDConfigSineWave>().m_uiRedHigh = ((iHexAsInt >> 16) & 0xFF);
			getActiveConfig<LEDConfigSineWave>().m_uiGreenHigh = ((iHexAsInt >> 8) & 0xFF);
			getActiveConfig<LEDConfigSineWave>().m_uiBlueHigh = ((iHexAsInt) & 0xFF);

			m_printFunc("High: " + String(getActiveConfig<LEDConfigSineWave>().m_uiRedHigh) + " - " + String(getActiveConfig<LEDConfigSineWave>().m_uiGreenHigh) + " - " + String(getActiveConfig<LEDConfigSineWave>().m_uiBlueHigh));
		}
		else if (rArguments[i].first == "rgbColorLow")
		{
			assertAndSetCustomConfig();
			int iHexAsInt;
			std::istringstream(rArguments[i].second.c_str()) >> std::hex >> iHexAsInt;

			getActiveConfig<LEDConfigSineWave>().m_uiRedLow = ((iHexAsInt >> 16) & 0xFF);
			getActiveConfig<LEDConfigSineWave>().m_uiGreenLow = ((iHexAsInt >> 8) & 0xFF);
			getActiveConfig<LEDConfigSineWave>().m_uiBlueLow = ((iHexAsInt) & 0xFF);

			m_printFunc("Low: " + String(getActiveConfig<LEDConfigSineWave>().m_uiRedLow) + " - " + String(getActiveConfig<LEDConfigSineWave>().m_uiGreenLow) + " - " + String(getActiveConfig<LEDConfigSineWave>().m_uiBlueLow));
		}
	}
}
