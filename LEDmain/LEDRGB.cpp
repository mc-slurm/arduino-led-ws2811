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
  rHTMLString += "<body><h1>Loesch LED RGB</h1>\n";

  rHTMLString += "<p>[<span id='datetime'></span>]</p>\n";
  
  if (m_bActive)
  {
	rHTMLString += "<p>Current LED State: on</p>\n";
    rHTMLString += "<p><a href=\"/rgb?action=off\"><button class=\"button button2\">OFF</button></a></p>\n";
  }
  else
  {
	rHTMLString += "<p>Current LED State: off</p>\n";
    rHTMLString += "<p><a href=\"/rgb?action=on\"><button class=\"button\">ON</button></a></p>\n";
  }

  rHTMLString += "<br style = “line-height:100px;”>";

  rHTMLString += "<table class =\"center\">\n";
  rHTMLString += "<tr>\n";
  rHTMLString += "  <td>Color</td>\n";
  rHTMLString += "  <td><input type=\"color\" id=\"rgbColor\" name=\"rgbColor\" value=\"#ffffff\" onchange=\"colorSel(this.id, this.value);\"></td>\n";
  rHTMLString += "</tr>\n";
  // rHTMLString += "<tr>\n";
  // rHTMLString += "  <td>Red</td>\n";
  // rHTMLString += "<td><input type=\"range\" id=\"red\" min=\"0\" max=\"255\" step=\"1.0\" value=\"" + String(m_iRed) + "\" oninput=\"numRed = this.value; updateVal(this.id, this.value)\" onchange=\"numRed = this.value; updateVal(this.id, this.value)\">\n";
  // rHTMLString += "    <output id=\"numRed\">0</output></td>\n";
  // rHTMLString += "</tr>\n";
  // rHTMLString += "<tr>\n";
  // rHTMLString += "  <td>Green</td>\n";
  // rHTMLString += "<td><input type=\"range\" id=\"green\" min=\"0\" max=\"255\" step=\"1.0\" value=\"" + String(m_iGreen) + "\" oninput=\"numGreen = this.value; updateVal(this.id, this.value)\" onchange=\"numGreen = this.value; updateVal(this.id, this.value)\">\n";
  // rHTMLString += "    <output id=\"numGreen\">0</output></td>\n";
  // rHTMLString += "</tr>\n";
  // rHTMLString += "<tr>\n";
  // rHTMLString += "  <td>Blue</td>\n";
  // rHTMLString += "<td><input type=\"range\" id=\"blue\" min=\"0\" max=\"255\" step=\"1.0\" value=\"" + String(m_iBlue) + "\" oninput=\"numBlue = this.value; updateVal(this.id, this.value)\" onchange=\"numBlue = this.value; updateVal(this.id, this.value)\">\n";
  // rHTMLString += "    <output id=\"numBlue\">0</output></td>\n";
  // rHTMLString += "</tr>\n";
  rHTMLString += "</table>\n";
  
  rHTMLString += "<p>Color Hex Value <span id=\"colHexValue\"></span></p>\n";
  
  
  rHTMLString += "\n";
  
  // rHTMLString += "<script>\n";
  // rHTMLString += "function updateVal(curId, newvalue) {\n";
  // rHTMLString += "window.location = 'http://10.254.192.17/rgb?' + curId + '=' + newvalue\n";
  // rHTMLString += "}\n";
  // rHTMLString += "</script>\n";

  rHTMLString += "<script>\n";
  rHTMLString += "function colorSel(curId, newvalue) {\n";
  rHTMLString += "var postText = newvalue.replace('#', '');";
  rHTMLString += "window.location = 'http://10.254.192.17/rgb?' + curId + '=' + postText\n";
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
  rHTMLString += "  document.getElementById(\"colHexValue\").innerHTML = rgbToHex(" + String(m_iRed) + "," + String(m_iGreen) + "," + String(m_iBlue) + ");\n";
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
