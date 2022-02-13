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
  rHTMLString = "<!DOCTYPE html>\n";
  rHTMLString += "<html>\n";
  rHTMLString += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
  rHTMLString += "<link rel=\"icon\" href=\"data:,\">\n";
  // CSS to style the on/off buttons 
  // Feel free to change the background-color and font-size attributes to fit your preferences
  rHTMLString += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  rHTMLString += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;\n";
  rHTMLString += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}\n";
  rHTMLString += ".center { margin-left: auto; margin-right: auto;}\n";
  rHTMLString += ".button2 {background-color: #555555;}</style></head>\n";
  
  // Web Page Heading
  rHTMLString += "<body><h1>Loesch LED Sine Wave</h1>\n";

  rHTMLString += "<p>[<span id='datetime'></span>]</p>\n";
  
  if (m_bActive)
  {
	rHTMLString += "<p>Current LED State: on</p>\n";
    rHTMLString += "<p><a href=\"/sinewave?action=off\"><button class=\"button button2\">OFF</button></a></p>\n";
  }
  else
  {
	rHTMLString += "<p>Current LED State: off</p>\n";
    rHTMLString += "<p><a href=\"/sinewave?action=on\"><button class=\"button\">ON</button></a></p>\n";
   }

  rHTMLString += "<br style = “line-height:100px;”>";
  
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
  
  rHTMLString += "<script>\n";
  rHTMLString += "function updateVal(curId, newvalue) {\n";
  rHTMLString += "var postText = newvalue.replace('#', '');";
  rHTMLString += "window.location = 'http://10.254.192.17/sinewave?' + curId + '=' + postText\n";
  rHTMLString += "}\n";
  rHTMLString += "</script>\n";
  
  rHTMLString += "<script>\n";
  rHTMLString += "function componentToHex(c) {\n";
  rHTMLString += "var hex = c.toString(16);\n";
  rHTMLString += "return hex.length == 1 ? \"0\" + hex : hex;\n";
  rHTMLString += "}\n";
  rHTMLString += "</script>\n";
  rHTMLString += "<script>\n";
  rHTMLString += "function rgbToHex(r, g, b) {\n";
  rHTMLString += "return \"#\" + componentToHex(r) + componentToHex(g) + componentToHex(b);\n";
  rHTMLString += "}\n";
  rHTMLString += "</script>\n";
  
  rHTMLString += "<script>\n";
  rHTMLString += "  document.getElementById(\"rgbColorHigh\").value = rgbToHex(" + String(m_iRedHigh) + "," + String(m_iGreenHigh) + "," + String(m_iBlueHigh) + ");\n";
  rHTMLString += "  document.getElementById(\"rgbColorLow\").value = rgbToHex(" + String(m_iRedLow) + "," + String(m_iGreenLow) + "," + String(m_iBlueLow) + ");\n";
  // rHTMLString += "  document.getElementById(\"colHexValue\").innerHTML = rgbToHex(" + String(m_iRedLow) + "," + String(m_iGreenLow) + "," + String(m_iBlueLow) + ");\n";
  rHTMLString += "</script>\n";
  
  // rHTMLString += "<script>\n";
  // rHTMLString += "function updateVal(curId, newvalue) {\n";
  // rHTMLString += "window.location = 'http://10.254.192.17/sinewave?' + curId + '=' + newvalue\n";
  // rHTMLString += "}\n";
  // rHTMLString += "</script>\n";
  /*
  rHTMLString += "<script>\n";
  rHTMLString += "function updateVal(newvalue) {\n";
  rHTMLString += "var request = new XMLHttpRequest()\n";
  //rHTMLString += "var url = \"/\" + set?r + \"=\" + value\n";
  rHTMLString += "request.open(\"GET\", \"http://10.254.192.17/set?r=\")\n";
//  rHTMLString += "request.send()\n";
  rHTMLString += "request.setRequestHeader(\"Content-Type\", \"application/x-www-form-urlencoded\")\n";
  rHTMLString += "request.send(value)\n";
  rHTMLString += "</script>\n";
*/

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
