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
  rHTMLString += ".button2 {background-color: #555555;}\n";
  rHTMLString += ".button3 {background-color: #ff0000;}</style></head>\n";
  
  // Web Page Heading
  rHTMLString += "<body><h1>Loesch LED Shot</h1>\n";

  rHTMLString += "<p>[<span id='datetime'></span>]</p>\n";
  
  if (m_bActive)
  {
	rHTMLString += "<p>Current LED State: on</p>\n";
    rHTMLString += "<p><a href=\"/shot?action=off\"><button class=\"button button2\">OFF</button></a></p>\n";
  }
  else
  {
	rHTMLString += "<p>Current LED State: off</p>\n";
    rHTMLString += "<p><a href=\"/shot?action=on\"><button class=\"button\">ON</button></a></p>\n";
   }

  rHTMLString += "<br style = “line-height:100px;”>";
  
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
  rHTMLString += "function updateVal(curId, newvalue) {\n";
  rHTMLString += "var postText = newvalue.replace('#', '');";
  rHTMLString += "window.location = 'http://10.254.192.17/shot?' + curId + '=' + postText\n";
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
  rHTMLString += "  document.getElementById(\"rgbColor\").value = rgbToHex(" + String(m_iRed) + "," + String(m_iGreen) + "," + String(m_iBlue) + ");\n";
  rHTMLString += "</script>\n";
  
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
