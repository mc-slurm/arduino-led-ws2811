#include "LEDBase.h"
#include <Arduino.h>
#include "FastLED.h"

void LEDBase::SetBlack(CRGB* leds, int iNumLEDs)
{
	for(int j = 0; j < iNumLEDs; j++)
	{
		leds[j] = CRGB::Black;
	}
}

void LEDBase::CreateHTMLHeader(const String& rTitle, String& rHTMLString)
{
	rHTMLString = "<!DOCTYPE html>\n";
	rHTMLString += "<html>\n";
	rHTMLString += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
	rHTMLString += "<link rel=\"icon\" href=\"data:,\">\n";
	// CSS to style the on/off buttons 
	rHTMLString += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
	rHTMLString += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;\n";
	rHTMLString += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}\n";
	rHTMLString += ".center { margin-left: auto; margin-right: auto;}\n";
	rHTMLString += ".button2 {background-color: #555555;}\n";
	rHTMLString += ".button3 {background-color: #ff0000;}</style></head>\n";

	// Web Page Heading
	rHTMLString += "<body><h1>" + rTitle + "</h1>\n";

	rHTMLString += "<p>[<span id='datetime'></span>]</p>\n";
}

void LEDBase::CreateHTMLOnOffButtons(const String& rSubPageLink, bool bActive, String& rHTMLString)
{
	if (bActive)
	{
		rHTMLString += "<p>Current LED State: on</p>\n";
		rHTMLString += "<p><a href=\"" + rSubPageLink + "?action=off\"><button class=\"button button2\">OFF</button></a></p>\n";
	}
	else
	{
		rHTMLString += "<p>Current LED State: off</p>\n";
		rHTMLString += "<p><a href=\"" + rSubPageLink + "?action=on\"><button class=\"button\">ON</button></a></p>\n";
	}	

	rHTMLString += "<br style = “line-height:100px;”>";
}

void LEDBase::CreateHTMLFooter(String& rHTMLString)
{
	rHTMLString += "<p></p>\n";
	rHTMLString += "<p></p>\n";
	rHTMLString += "<a href=\"/\">- HOME -</a>\n";

	rHTMLString += "</body></html>\n";

	// The HTTP response ends with another blank line
	rHTMLString += "\n";	
}

void LEDBase::CreateHTMLRGBToHexFunction(String& rHTMLString)
{
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
}

void LEDBase::CreateHTMLUpdateValueFunction(const String& rURL, const String& rSubPage, String& rHTMLString)
{
	rHTMLString += "<script>\n";
	rHTMLString += "function updateVal(curId, newvalue) {\n";
	rHTMLString += "var postText = newvalue.replace('#', '');";
	rHTMLString += "window.location = 'http://" + rURL + rSubPage + "?' + curId + '=' + postText\n";
	rHTMLString += "}\n";
	rHTMLString += "</script>\n";
}

void LEDBase::CreateHTMLDateTimeFunction(String& rHTMLString)
{
	rHTMLString += "<script>\n";
	rHTMLString += "var dt = new Date()\n";
	rHTMLString += "document.getElementById(\"datetime\").innerHTML = ((\"0\"+dt.getDate()).slice(-2)) +\".\"+ ((\"0\"+(dt.getMonth()+1)).slice(-2)) +\".\"+ (dt.getFullYear()) +\" \"+ ((\"0\"+dt.getHours()).slice(-2)) +\":\"+ ((\"0\"+dt.getMinutes()).slice(-2))\n";
	rHTMLString += "</script>\n";
}