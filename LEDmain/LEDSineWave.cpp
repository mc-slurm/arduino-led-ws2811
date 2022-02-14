#include "LEDSineWave.h"
#include <Arduino.h>
#include "FastLED.h"
#include <sstream>

LEDSineWave::LEDSineWave(void)
{
	
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
	rHTMLString += "<td><input type=\"range\" id=\"k\" min=\"0\" max=\"1\" step=\"0.01\" value=\"" + String(m_fk) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
	rHTMLString += "    <label for=\"k\">" + String(m_fk) + "</label>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>wave speed</td>\n";
	rHTMLString += "<td><input type=\"range\" id=\"w\" min=\"0\" max=\"5\" step=\"0.1\" value=\"" + String(m_fw) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
	rHTMLString += "    <label for=\"w\">" + String(m_fw) + "</label>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>wave shift</td>\n";
	rHTMLString += "<td><input type=\"range\" id=\"o\" min=\"0\" max=\"3\" step=\"0.1\" value=\"" + String(m_fo) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
	rHTMLString += "    <label for=\"o\">" + String(m_fo) + "</label>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>time step</td>\n";
	rHTMLString += "<td><input type=\"range\" id=\"dt\" min=\"0\" max=\"1\" step=\"0.001\" value=\"" + String(m_fdT) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
	rHTMLString += "    <label for=\"dt\">" + String(m_fdT) + "</label>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>Color high</td>\n";
	rHTMLString += "  <td><input type=\"color\" id=\"rgbColorHigh\" name=\"rgbColorHigh\" value=\"#ffffff\" onchange=\"updateVal(this.id, this.value);\"></td>\n";
	rHTMLString += "</tr>\n";  
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>Color low</td>\n";
	rHTMLString += "  <td><input type=\"color\" id=\"rgbColorLow\" name=\"rgbColorLow\" value=\"#ffffff\" onchange=\"updateVal(this.id, this.value);\"></td>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "</table>\n";
  
	CreateHTMLRGBToHexFunction(rHTMLString);
	CreateHTMLUpdateValueFunction(m_URL, GetSubPageLink(), rHTMLString);
	CreateHTMLDateTimeFunction(rHTMLString);
  
	rHTMLString += "<script>\n";
	rHTMLString += "  document.getElementById(\"rgbColorHigh\").value = rgbToHex(" + String(m_iRedHigh) + "," + String(m_iGreenHigh) + "," + String(m_iBlueHigh) + ");\n";
	rHTMLString += "  document.getElementById(\"rgbColorLow\").value = rgbToHex(" + String(m_iRedLow) + "," + String(m_iGreenLow) + "," + String(m_iBlueLow) + ");\n";
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
		float fVal = sinf(m_fk * (float)j - m_fw * m_fTime + m_fo)/2.f + 0.5f;

		float fRed = fVal * (float)m_iRedHigh + (1.f - fVal) * (float)m_iRedLow;
		float fGreen = fVal * (float)m_iGreenHigh + (1.f - fVal) * (float)m_iGreenLow;
		float fBlue = fVal * (float)m_iBlueHigh + (1.f - fVal) * (float)m_iBlueLow;

		leds[j] =  CRGB((uint8_t)fRed, (uint8_t)fGreen, (uint8_t)fBlue);
//		leds[j] =  CRGB(120, (uint8_t)((float)60 * fVal), 0);
	}
	m_fTime += m_fdT;
}

String LEDSineWave::GetSubPage(void) const
{
	return "Sine Wave";
}

String LEDSineWave::GetSubPageLink(void) const
{
	return "/sinewave";
}

void LEDSineWave::onEvent(std::vector<std::pair<String, String>>& rArguments)
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
		else if (rArguments[i].first == "k")
		{
			m_fk = rArguments[i].second.toFloat();
		}
		else if (rArguments[i].first == "w")
		{
			m_fw = rArguments[i].second.toFloat();
		}
		else if (rArguments[i].first == "o")
		{
			m_fo = rArguments[i].second.toFloat();
		}
		else if (rArguments[i].first == "dt")
		{
			m_fdT = rArguments[i].second.toFloat();
		}
		else if (rArguments[i].first == "rgbColorHigh")
		{
			int iHexAsInt;
			std::istringstream(rArguments[i].second.c_str()) >> std::hex >> iHexAsInt;

			m_iRedHigh = ((iHexAsInt >> 16) & 0xFF);
			m_iGreenHigh = ((iHexAsInt >> 8) & 0xFF);
			m_iBlueHigh = ((iHexAsInt) & 0xFF);

			m_printFunc("High: " + String(m_iRedHigh) + " - " + String(m_iGreenHigh) + " - " + String(m_iBlueHigh));
		}
		else if (rArguments[i].first == "rgbColorLow")
		{
			int iHexAsInt;
			std::istringstream(rArguments[i].second.c_str()) >> std::hex >> iHexAsInt;

			m_iRedLow = ((iHexAsInt >> 16) & 0xFF);
			m_iGreenLow = ((iHexAsInt >> 8) & 0xFF);
			m_iBlueLow = ((iHexAsInt) & 0xFF);

			m_printFunc("Low: " + String(m_iRedLow) + " - " + String(m_iGreenLow) + " - " + String(m_iBlueLow));
		}
	}
}
