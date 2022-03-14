#include "LEDController.h"
#include "Arduino.h"
#include "FastLED.h"
#include "LEDRGB.h"
#include "LEDFire.h"
#include "LEDSineWave.h"
#include "LEDShot.h"
#include "GlobalDefs.h"
#include "StreamEEPROM.h"
#include <sstream>
#include <string>

#define CHIPSET    	WS2811
#define COLOR_ORDER GRB
#define LED_PIN_27 27

const String globalPage = "/global";

struct LEDController::SData
{
	SData(void) {}
	uint8_t iLEDPin = 0;
	int iNumLEDs = 0;
	uint8_t iBrightness = 0;
	int iUpdateRate = 0; // per second
	String url = "";
	bool bSchedulerEnabled = false;
	
	uint8_t uiHour = 0;
	uint8_t uiMinute = 0;
	uint8_t uiStartHour = 0;
	uint8_t uiStartMinute = 0;
	uint8_t uiEndHour = 0;
	uint8_t uiEndMinute = 0;
	uint8_t uiLastActive = 0;
	
	std::vector<std::unique_ptr<LEDBase>> availableLEDFunctions;
	CRGB* pLeds = nullptr;
};

LEDController::LEDController(void)
{
}

LEDController::~LEDController(void)
{
	if (m_spData != nullptr)
	{
		if(m_spData->pLeds != nullptr)
		{
			delete[] m_spData->pLeds;
		}
	}
}

void LEDController::CreateHTML(String& rHTMLString)
{
	// Display the HTML web page
	rHTMLString = "<!DOCTYPE html>\n";
	rHTMLString += "<html>\n";
	rHTMLString += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
	rHTMLString += "<link rel=\"icon\" href=\"data:,\">\n";
	// CSS to style the on/off buttons
	rHTMLString += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
	rHTMLString += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;\n";
	rHTMLString += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}\n";
	rHTMLString += ".center { margin-left: auto; margin-right: auto;}\n";
	rHTMLString += "th, td { border-bottom: 1px solid lightgrey; text-align: left; padding: 15px; }\n";
	rHTMLString += "table { border-collapse: collapse; width: 100%; }\n";
	rHTMLString += ".dropdown { display: inline-block; position: relative; }\n";
	rHTMLString += ".dropdown-content { display: none; position: absolute; width: 100%; overflow: auto; box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2); }\n";
	rHTMLString += ".dropdown:hover .dropdown-content { display: block; }\n";
	rHTMLString += ".dropdown-content a { display: block; color: #000000; background-color: #FFFFFF; padding: 5px; text-decoration: none; }\n";
	rHTMLString += ".dropdown-content a:hover { color: #FFFFFF; background-color: #00A4BD; }\n";
	rHTMLString += ".button3 {background-color: #555555; border: none; color: white; padding: 5px 5px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer;}\n";
	rHTMLString += ".button2 {background-color: #555555;}</style></head>\n";

	// Web Page Heading
	rHTMLString += "<body><h1>LED Home</h1>\n";

	rHTMLString += "<p>[<span id='datetime'></span>]</p>\n";
	rHTMLString += "<p></p>\n";

	rHTMLString += "<table class =\"center\">\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <th>Description</th>\n";
	rHTMLString += "  <th>Property</th>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>Theme</td>\n";
	rHTMLString += "  <td>\n";

	rHTMLString += "<div class=\"dropdown\">\n";
	rHTMLString += "<button class=\"button3\">Select theme...</button>\n";
	rHTMLString += "<div class=\"dropdown-content\">\n";
	for (int i = 0; i < m_spData->availableLEDFunctions.size(); ++i)
	{
		rHTMLString += "<a href=\"" + m_spData->availableLEDFunctions[i]->GetSubPageLink() + "\">" + m_spData->availableLEDFunctions[i]->GetSubPage() + "</a>\n";
	}
	rHTMLString += "</div>\n";
	rHTMLString += "</div>\n";

	rHTMLString += "  </td>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>Num LEDs</td>\n";
	rHTMLString += "  <td>\n";
	rHTMLString += "    <input type=\"range\" id=\"numLEDs\" name=\"numLEDs\" min=\"0\" max=\"512\" value=\"" + String(m_spData->iNumLEDs) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
	rHTMLString += "    <label for=\"numLEDs\">" + String(m_spData->iNumLEDs) + "</label>\n";
	rHTMLString += "  </td>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>Brightness</td>\n";
	rHTMLString += "  <td>\n";
	rHTMLString += "    <input type=\"range\" id=\"brightness\" name=\"brightness\" min=\"0\" max=\"255\" value=\"" + String(m_spData->iBrightness) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
	rHTMLString += "    <label for=\"brightness\">" + String(m_spData->iBrightness) + "</label>\n";
	rHTMLString += "  </td>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td><input type=\"checkbox\" id=\"scheduling\" name=\"scheduling\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\"" + (m_spData->bSchedulerEnabled ? String(" checked") : String("")) + ">\n";
	rHTMLString += "  <label for=\"scheduling\">Scheduler</label>\n";
	rHTMLString += "  </td>\n";
	rHTMLString += "  <td>\n";
	
	auto adaptTimeVal = [](uint8_t uiVal) -> String
	{
		if (uiVal < 10)
		{
			return String(0) + String(uiVal);
		}
		return String(uiVal);
	};
	
	if (m_spData->bSchedulerEnabled)
	{
		rHTMLString += "    <br>";
		rHTMLString += "    <input type=\"time\" id=\"fromTime\" name=\"fromTime\" value=\"" + adaptTimeVal(m_spData->uiStartHour) + ":" + adaptTimeVal(m_spData->uiStartMinute) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
		rHTMLString += "    <br>";
		rHTMLString += "    <input type=\"time\" id=\"toTime\" name=\"toTime\" value=\"" + adaptTimeVal(m_spData->uiEndHour) + ":" + adaptTimeVal(m_spData->uiEndMinute) + "\" oninput=\"updateVal(this.id, this.value)\" onchange=\"updateVal(this.id, this.value)\">\n";
	}
	rHTMLString += "  </td>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "<tr>\n";
	rHTMLString += "  <td>All configurations</td>\n";
	rHTMLString += "  <td>\n";
	rHTMLString += "    <button class=\"button3\" id=\"saveAll\" type=\"button\" onclick=\"configAction(this.id);\">Save</button>\n";
	rHTMLString += "    <button class=\"button3\" id=\"loadAll\" type=\"button\" onclick=\"configAction(this.id);\">Load</button>\n";
	rHTMLString += "  </td>\n";
	rHTMLString += "</tr>\n";
	rHTMLString += "</table>\n";

	rHTMLString += "<script>\n";
	rHTMLString += "	function configAction(curId) {\n";
	rHTMLString += "        var destURL = 'http://" + m_spData->url + "?configAction=' + curId;\n";
	rHTMLString += "		alert(destURL);\n";
	rHTMLString += "		window.location.href = destURL\n";
	rHTMLString += "	}\n";
	rHTMLString += "</script>\n";

	LEDBase::CreateHTMLDateTimeFunction(rHTMLString);
	LEDBase::CreateHTMLUpdateValueFunction(m_spData->url, globalPage, rHTMLString);
	
	LEDBase::CreateHTMLFooter(rHTMLString);
}

void LEDController::SetNumLEDs(int iNumLEDs)
{
	if (m_printFunc != nullptr)
		m_printFunc("LEDController::SetNumLEDs");

	bool bChange = (iNumLEDs > 0 && m_spData->uiLastActive < iNumLEDs);
	if (bChange)
	{
		// Set all black.
		LEDBase::SetBlack(m_spData->pLeds, m_spData->iNumLEDs);
		m_spData->availableLEDFunctions[m_spData->uiLastActive]->SetActive(false);
	}
	
	m_spData->iNumLEDs = iNumLEDs;
	if (m_spData->pLeds != nullptr)
	{
		delete[] m_spData->pLeds;
	}
	m_spData->pLeds = new CRGB[m_spData->iNumLEDs];
	registerLEDs();
	
	if (bChange)
	{
		m_spData->availableLEDFunctions[m_spData->uiLastActive]->SetActive(true);
	}
}

void LEDController::SetBrightness(uint8_t iBrightness)
{
	m_spData->iBrightness = iBrightness;
	FastLED.setBrightness( m_spData->iBrightness );
}

void LEDController::SetUpdateRate(int iUpdateRate)
{
	m_spData->iUpdateRate = iUpdateRate;
}

void LEDController::SetURL(const String& rURL)
{
	m_spData->url = rURL;
}

void LEDController::SetTime(uint8_t uiHour, uint8_t uiMinute)
{
	m_spData->uiHour = uiHour;
	m_spData->uiMinute = uiMinute;
}


std::vector<String> LEDController::GetSubPageLinks(void) const
{
	std::vector<String> result;
	for (int i = 0; i < m_spData->availableLEDFunctions.size(); ++i)
	{
		result.push_back(m_spData->availableLEDFunctions[i]->GetSubPageLink());
	}
	return std::move(result);
}

void LEDController::registerLEDs(void)
{
	if (m_spData->iLEDPin == 27)
	{
		if (m_printFunc != nullptr)
			m_printFunc("LEDController::addLeds");
		FastLED.addLeds<CHIPSET, LED_PIN_27, COLOR_ORDER>(m_spData->pLeds, m_spData->iNumLEDs).setCorrection( TypicalLEDStrip );
	}
	FastLED.setBrightness( m_spData->iBrightness );
}

void LEDController::Setup(const String& rURL, uint8_t iLEDPin, int iNumLEDs, uint8_t iBrightness, int iUpdateRate)
{
	if (m_printFunc != nullptr)
		m_printFunc("LEDController::Setup");

	m_spData = make_unique<SData>();
	
	m_spData->iLEDPin = iLEDPin;
	m_spData->iNumLEDs = iNumLEDs;
	m_spData->iBrightness = iBrightness;
	m_spData->iUpdateRate = iUpdateRate;
	m_spData->url = rURL;
	m_spData->uiLastActive = (uint8_t)m_spData->iNumLEDs;
	
	SetNumLEDs(m_spData->iNumLEDs);
	
	if (m_printFunc != nullptr)
		m_printFunc("LEDController::registerLEDFunctions");
	m_spData->availableLEDFunctions.push_back(make_unique<LEDRGB>());
	m_spData->availableLEDFunctions.push_back(make_unique<LEDFire>());
	m_spData->availableLEDFunctions.push_back(make_unique<LEDSineWave>());
	m_spData->availableLEDFunctions.push_back(make_unique<LEDShot>());
	
	for (int i = 0; i < m_spData->availableLEDFunctions.size(); ++i)
	{
		if (m_printFunc != nullptr)
		{
			m_spData->availableLEDFunctions[i]->RegisterPrintFunction(m_printFunc);
		}
		m_spData->availableLEDFunctions[i]->SetUpdateFrequency(m_spData->iUpdateRate);
		m_spData->availableLEDFunctions[i]->SetURL(m_spData->url);
	}
	
	// LEDs
	registerLEDs();

	// Set all black.
	LEDBase::SetBlack(m_spData->pLeds, m_spData->iNumLEDs);
}

void LEDController::Loop()
{
	auto getTime = [](uint8_t uiHour, uint8_t uiMinute) -> uint32_t
	{
		return (uint32_t)uiHour*24 + (uint32_t)uiMinute;
	};

	// scheduler enabled and active led function
	if (m_spData->bSchedulerEnabled)
	{
		uint32_t uiStartTime = getTime(m_spData->uiStartHour, m_spData->uiStartMinute);
		uint32_t uiEndTime = getTime(m_spData->uiEndHour, m_spData->uiEndMinute);
		uint32_t uiCurTime = getTime(m_spData->uiHour, m_spData->uiMinute);
		

		// end time smaller than starttime
		bool bIsInRange = true;
		if (uiEndTime != uiStartTime)
		{
			if (uiEndTime < uiStartTime)
			{
				bIsInRange = !((uiEndTime <= uiCurTime) && (uiCurTime < uiStartTime));
			}
			else
			{
				bIsInRange = (uiStartTime <= uiCurTime) && (uiCurTime < uiEndTime);
			}
		}
		
		// static uint32_t uiDebugVal = 0;
		// if (uiDebugVal != uiCurTime)
		// {
			// uiDebugVal = uiCurTime;
			// m_printFunc("last index: " + String(m_spData->uiLastActive) + "\tlast active: " + (m_spData->availableLEDFunctions[m_spData->uiLastActive]->IsActive() ? "yes" : "no"));
			// m_printFunc("cur: " + String(m_spData->uiHour) + ":" + String(m_spData->uiMinute) + "start: " + String(m_spData->uiStartHour) + ":" + String(m_spData->uiStartMinute) + "\tend: " + String(m_spData->uiEndHour) + ":" + String(m_spData->uiEndMinute));
			// m_printFunc("start: " + String(uiStartTime) + "\tend: " + String(uiEndTime) + "\tcur: " + String(uiCurTime) + "\tin range: " + (bIsInRange ? "yes" : "no"));
		// }
		
		if (!bIsInRange && m_spData->availableLEDFunctions[m_spData->uiLastActive]->IsActive())
		{
			m_printFunc("DISABLE! Index " + String(m_spData->uiLastActive) + "\tTime: " + String(m_spData->uiHour) + ":" + String(m_spData->uiMinute));
			m_spData->availableLEDFunctions[m_spData->uiLastActive]->SetActive(false);
			LEDBase::SetBlack(m_spData->pLeds, m_spData->iNumLEDs);
		}
		else if (bIsInRange && !m_spData->availableLEDFunctions[m_spData->uiLastActive]->IsActive())
		{
			m_printFunc("ENABLE! Index " + String(m_spData->uiLastActive) + "\tTime: " + String(m_spData->uiHour) + ":" + String(m_spData->uiMinute));
			m_spData->availableLEDFunctions[m_spData->uiLastActive]->SetActive(true);
		}
	}
	
	for (int i = 0; i < m_spData->availableLEDFunctions.size(); ++i)
	{
		if (m_spData->availableLEDFunctions[i]->IsActive())
		{
			m_spData->availableLEDFunctions[i]->UpdateLEDs(m_spData->pLeds, m_spData->iNumLEDs);
			m_spData->uiLastActive = (uint8_t)i;
		}
	}

	FastLED.show(); // display this frame
	FastLED.delay(1000 / m_spData->iUpdateRate);	
}

void LEDController::OnEvent(const String& rURL, std::vector<std::pair<String, String>>& rArguments, String& rHTMLString)
{
	if (rURL == "/global")
	{
		for (int i = 0; i < rArguments.size(); ++i)
		{
			if (rArguments[i].first == "numLEDs")
			{
				SetNumLEDs(rArguments[i].second.toInt());
			}
			else if (rArguments[i].first == "brightness")
			{
				SetBrightness(rArguments[i].second.toInt());
			}
			else if (rArguments[i].first == "fromTime")
			{
				std::stringstream ss(rArguments[i].second.c_str());

				std::string substr;
				getline(ss, substr, ':');
				m_spData->uiStartHour = atoi(substr.c_str());
				getline(ss, substr, ':');
				m_spData->uiStartMinute = atoi(substr.c_str());
				
				if (m_printFunc != nullptr)
				{
					m_printFunc("start time: " + String(m_spData->uiStartHour) + "-" + String(m_spData->uiStartMinute));
				}

			}
			else if (rArguments[i].first == "toTime")
			{
				std::stringstream ss(rArguments[i].second.c_str());
				std::string substr;
				getline(ss, substr, ':');
				m_spData->uiEndHour = atoi(substr.c_str());
				getline(ss, substr, ':');
				m_spData->uiEndMinute = atoi(substr.c_str());

				if (m_printFunc != nullptr)
				{
					m_printFunc("end time: " + String(m_spData->uiEndHour) + "-" + String(m_spData->uiEndMinute));
				}
			}
			else if (rArguments[i].first == "scheduling")
			{
				m_spData->bSchedulerEnabled = !m_spData->bSchedulerEnabled;
				//m_spData->bSchedulerEnabled = (rArguments[i].second == "on" ? true : false);
			}
			else if (rArguments[i].first == "configAction")
			{
				m_printFunc("configAction");
				if (rArguments[i].second == "saveAll")
				{
					m_printFunc("- saveAll");
					SaveConfigs();
				}
				else if (rArguments[i].second == "loadAll")
				{
					m_printFunc("- loadAll");
					LoadConfigs();
				}
			}
			CreateHTML(rHTMLString);
		}
		return;
	}
    // Find handler
    for (int i = 0; i < m_spData->availableLEDFunctions.size(); ++i)
    {
		if (m_spData->availableLEDFunctions[i]->GetSubPageLink() == rURL)
		{
			bool bIsActive = m_spData->availableLEDFunctions[i]->IsActive();
			m_spData->availableLEDFunctions[i]->OnEvent(rArguments);
	  
			// If LED has been activated, then deactivate all others
			if (!bIsActive && m_spData->availableLEDFunctions[i]->IsActive())
			{
				// Disable all others
				for (int j = 0; j < m_spData->availableLEDFunctions.size(); ++j)
				{
					if (i != j)
					{
						m_spData->availableLEDFunctions[j]->SetActive(false);
					}
				}
			}
			else if (bIsActive && !m_spData->availableLEDFunctions[i]->IsActive())
			{
				// Update LEDs if set inactive
				m_spData->availableLEDFunctions[i]->UpdateLEDs(m_spData->pLeds, m_spData->iNumLEDs);
			}
	  
			m_spData->availableLEDFunctions[i]->CreateHTML(rHTMLString);
	  
			break;
		}
    }
}

void LEDController::SaveConfigs(void) const
{
	m_printFunc("LEDController::SaveConfigs");
	try
	{
		StreamEEPROM stream;
		for (int i = 0; i < m_spData->availableLEDFunctions.size(); ++i)
		{
			m_spData->availableLEDFunctions[i]->Serialize(stream);
		}
		
		m_printFunc("Writing to EEPROM (size: " + String(stream.GetSize()) + ")");
		if (stream.GetSize() > 255)
		{
			throw 0;
		}
		stream.WriteEEPROM(0); // write all from EEPROM.
	}
	catch(...)
	{
		
	}
}

void LEDController::LoadConfigs(void)
{
	m_printFunc("LEDController::LoadConfigs");
	try
	{
		StreamEEPROM stream;
		stream.ReadEEPROM(0); // read all from EEPROM.
		{
			const StreamEEPROM& rStream = stream;
			for (int i = 0; i < m_spData->availableLEDFunctions.size(); ++i)
			{
				m_spData->availableLEDFunctions[i]->Serialize(rStream);
			}		
		}
	}
	catch(...)
	{
		
	}
}

