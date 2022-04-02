#include "LEDBase.h"
#include <Arduino.h>
#include "FastLED.h"
#include "StreamEEPROM.h"
#include "Logger.h"

const uint32_t c_uiVersion = 0;

void LEDBase::SetAll(CRGB* leds, int iNumLEDs, const CRGB& rColor)
{
	for(int j = 0; j < iNumLEDs; j++)
	{
		leds[j] = rColor;
	}
}

void LEDBase::SetBlack(CRGB* leds, int iNumLEDs)
{
	SetAll(leds, iNumLEDs, CRGB::Black);
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
	
	rHTMLString += ".dropdown { display: inline-block; position: relative; }\n";
	rHTMLString += ".dropdown-content { display: none; position: absolute; width: 100%; overflow: auto; box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2); }\n";
	rHTMLString += ".dropdown:hover .dropdown-content { display: block; }\n";
	rHTMLString += ".dropdown-content a { display: block; color: #000000; background-color: #FFFFFF; padding: 5px; text-decoration: none; }\n";
	rHTMLString += ".dropdown-content a:hover { color: #FFFFFF; background-color: #00A4BD; }\n";
	rHTMLString += ".button4 {background-color: #555555; border: none; color: white; padding: 5px 5px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer;}\n";
	rHTMLString += ".button3 {background-color: #ff0000;}\n";
	rHTMLString += ".button2 {background-color: #555555;}</style></head>\n";

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

	rHTMLString += "<br style = \"line-height:100px;\">";
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
	rHTMLString += "var postText = newvalue.replace('#', '');\n";
	rHTMLString += "window.location = 'http://" + rURL + rSubPage + "?' + curId + '=' + postText\n";
	rHTMLString += "}\n";
	rHTMLString += "</script>\n";
}

void LEDBase::CreateHTMLAddConfigFunction(const String& rURL, const String& rSubPage, String& rHTMLString)
{
	rHTMLString += "<script>\n";
	rHTMLString += "	function addConfig() {\n";
	rHTMLString += "		var inputVal = document.getElementById(\"configname\").value;\n";
	rHTMLString += "        var destURL = 'http://" + rURL + rSubPage + "?" + getConfigCreateString() + "=' + inputVal;\n";
	//rHTMLString += "		alert(destURL);\n";
	rHTMLString += "		window.location.href = destURL\n";
	rHTMLString += "	}\n";
	rHTMLString += "</script>\n";
}

void LEDBase::CreateHTMLDateTimeFunction(String& rHTMLString)
{
	rHTMLString += "<script>\n";
	rHTMLString += "var dt = new Date()\n";
	rHTMLString += "document.getElementById(\"datetime\").innerHTML = ((\"0\"+dt.getDate()).slice(-2)) +\".\"+ ((\"0\"+(dt.getMonth()+1)).slice(-2)) +\".\"+ (dt.getFullYear()) +\" \"+ ((\"0\"+dt.getHours()).slice(-2)) +\":\"+ ((\"0\"+dt.getMinutes()).slice(-2))\n";
	rHTMLString += "</script>\n";
}

void LEDBase::CreateHTMLConfigTableRow(std::vector<std::unique_ptr<LEDConfigBase>>& rConfigs, const String& rSubPageLink, const String& rActiveConfigName, String& rHTMLString)
{
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>Config</td>\n";

	rHTMLString += "  <td>\n";
	rHTMLString += "<div class=\"dropdown\">\n";
	rHTMLString += "<button class=\"button4\">" + rActiveConfigName + "</button>\n";
	rHTMLString += "<div class=\"dropdown-content\">\n";
	for (int i = 0; i < rConfigs.size(); ++i)
	{
		rHTMLString += "<a href=\"" + rSubPageLink + "?" + getConfigActionString() + "=" + rConfigs[i]->GetName() + "\">" + rConfigs[i]->GetName() + "</a>\n";
	}
	rHTMLString += "</div>\n";
	rHTMLString += "<a href=\"" + rSubPageLink + "?" + getConfigDeleteString() + "=" + rActiveConfigName + "\" class=\"button4\">X</a>\n";
	rHTMLString += "</div>\n";
	rHTMLString += "  </td>\n";

	rHTMLString += "</tr>\n";	

	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>Save As</td>\n";
	rHTMLString += "    <td>\n";
	rHTMLString += "		<input type=\"text\" placeholder=\"enter name...\" id=\"configname\" size=\"16\">\n";
	rHTMLString += "		<button class=\"button4\" type=\"button\" onclick=\"addConfig();\">Save</button>\n";
	rHTMLString += "  </td>\n";
	rHTMLString += "</tr>\n";
}

String LEDBase::getConfigActionString(void)
{
	return "selectconfig";
}

String LEDBase::getConfigCreateString(void)
{
	return "createconfig";
}

String LEDBase::getConfigDeleteString(void)
{
	return "deleteconfig";
}

bool LEDBase::isConfigNameReserved(const String& rName) const
{
	if ((rName == "custom") ||
		(rName == "default"))
	{
		return true;
	}
	return false;
}
bool LEDBase::isConfigNameExists(const String& rName) const
{
	for (int j = 0; j < m_configurations.size(); ++j)
	{
		if (m_configurations[j]->GetName() == rName)
		{
			return true;
		}
	}
	return false;
}

void LEDBase::assertAndSetCustomConfig(void)
{
	LED_LOG("assertAndSetCustomConfig");
	if (!isConfigNameExists("custom"))
	{
		LED_LOG("\t- creating config");
		m_configurations.push_back(createConfig("custom"));
	}
	for (int j = 0; j < m_configurations.size(); ++j)
	{
		if (m_configurations[j]->GetName() == "custom")
		{
			if (j != m_uiActiveConfig)
			{
				// copy current settings.
				*m_configurations[j] = *m_configurations[m_uiActiveConfig];
				m_configurations[j]->SetName("custom");
				// set custom active
				LED_LOG("old config : " + String(m_uiActiveConfig) + ", new config: " + String(j));
				m_uiActiveConfig = j;
				break;
			}
		}
	}
}

void LEDBase::onEvent(std::vector<std::pair<String, String>>& rArguments)
{
	for (int i = 0; i < rArguments.size(); ++i)
	{
		if (rArguments[i].first == getConfigActionString())
		{
			for (int j = 0; j < m_configurations.size(); ++j)
			{
				if (m_configurations[j]->GetName() == rArguments[i].second)
				{
					m_uiActiveConfig = static_cast<uint32_t>(j);
					break;
				}
			}
		}
		else if (rArguments[i].first == getConfigDeleteString())
		{
			if (m_configurations.size() > 0) // at least one elem must exist
			{
				for (int j = 0; j < m_configurations.size(); ++j)
				{
					if (m_configurations[j]->GetName() == rArguments[i].second)
					{
						m_configurations.erase(m_configurations.begin() + j);
						m_uiActiveConfig = 0;
						break;
					}
				}
			}
		}
		else if (rArguments[i].first == getConfigCreateString())
		{
			if (!isConfigNameReserved(String(rArguments[i].second)))
			{
				m_configurations[m_uiActiveConfig]->SetName(String(rArguments[i].second));
				// bool bActiveIsCustom = (m_configurations[m_uiActiveConfig]->GetName() == "custom");

				// m_configurations.push_back(createConfig(String(rArguments[i].second)));

				// LED_LOG("copy config from: " + m_configurations[m_uiActiveConfig]->GetName() + " to: " + m_configurations[m_configurations.size() - 1]->GetName());
				// // copy settings
				// *m_configurations[m_configurations.size() - 1] = *m_configurations[m_uiActiveConfig];
				// m_configurations[m_configurations.size() - 1]->SetName(String(rArguments[i].second));

				// if (bActiveIsCustom)
				// {
					// // remove custom.
					// m_configurations.erase(m_configurations.begin() + m_uiActiveConfig);
				// }
				// m_uiActiveConfig = m_configurations.size() - 1;
			}
		}
	}
}

void LEDBase::Serialize(StreamEEPROMWrite& rStream) const
{
	LED_LOG("LEDBase::Serialize (write)");
	LED_LOG("write configuration version" + String(c_uiVersion));
	rStream.Write(c_uiVersion);
	LED_LOG("write num configurations " + String(m_configurations.size()));
	rStream.Write((uint32_t)m_configurations.size());
	for (int j = 0; j < m_configurations.size(); ++j)
	{
		LED_LOG("writing configuration " + m_configurations[j]->GetName() + " index " + String(j));
		rStream.Write(m_configurations[j]->GetName());
		m_configurations[j]->Serialize(rStream);
	}
	
	rStream.Write(m_uiActiveConfig);
}

void LEDBase::Serialize(const StreamEEPROMRead& rStream)
{
	LED_LOG("LEDBase::Serialize (read)");
	
	m_configurations.clear();
	
	LED_LOG("read version");
	uint32_t uiVersion = 0;
	rStream.Read(uiVersion);
	if (uiVersion >= 0)
	{
		LED_LOG("read num configurations");
		uint32_t uiNumConfigurations = 0;
		rStream.Read(uiNumConfigurations);
		
		for (int j = 0; j < uiNumConfigurations; ++j)
		{
			LED_LOG("read config name" + String(j));
			String configName;
			rStream.Read(configName);
			LED_LOG("create config " + String(j));
			m_configurations.push_back(createConfig(configName));
			LED_LOG("load config" + String(j));
			m_configurations[j]->Serialize(rStream);
		}
	}

	rStream.Read(m_uiActiveConfig);
}
