#include "LEDController.h"
#include "Arduino.h"
#include "FastLED.h"
#include "LEDRGB.h"
#include "LEDFire.h"
#include "LEDSineWave.h"
#include "LEDShot.h"
#include "GlobalDefs.h"


#define CHIPSET    	WS2811
#define COLOR_ORDER GRB
#define LED_PIN_27 27

struct LEDController::SData
{
	SData(void) {}
	uint8_t iLEDPin = 0;
	int iNumLEDs = 0;
	uint8_t iBrightness = 0;
	int iUpdateRate = 0; // per second
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
  // Feel free to change the background-color and font-size attributes to fit your preferences
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
  rHTMLString += "<body><h1>Loesch LED test</h1>\n";

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
  /*
  rHTMLString += "    <select name=\"theme\" onchange=\"location = this.value;\">\n";
  rHTMLString += "<option value=\"\">- select -</option>\n";
  for (int i = 0; i < NUM_LED_FUNCTIONS; ++i)
  {
    rHTMLString += "<option value=\"" + availableLEDFunctions[i]->GetSubPage() + "\">" + availableLEDFunctions[i]->GetSubPage() + "</option>\n";
  }
  rHTMLString += "    </select>\n";
*/
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
  rHTMLString += "    <input type=\"range\" id=\"numLEDs\" name=\"numLEDs\" min=\"0\" max=\"1024\" oninput=\"num.value = this.value\"\">\n";
  rHTMLString += "    <output id=\"num\">0</output>\n";
  rHTMLString += "  </td>\n";
  rHTMLString += "</tr>\n";
  rHTMLString += "<tr>\n";
  rHTMLString += "  <td>Scheduling</td>\n";
  rHTMLString += "  <td>\n";
//  rHTMLString += "    <label for=\"schedulingCheckBox\">enabled</label>\n";
//  rHTMLString += "    <input type=\"checkbox\" name=\"schedulingCheckBox\" id=\"schedulingCheckBox\" onclick=\"scheduleClicked(this)\">\n";
  rHTMLString += "    <br>";
//  rHTMLString += "    <label for=\"fromTime\">From</label>\n";
  rHTMLString += "    <input type=\"time\" id=\"fromTime\" name=\"fromTime\">\n";
  rHTMLString += "    <br>";
//  rHTMLString += "    <label for=\"toTime\">to</label>\n";
  rHTMLString += "    <input type=\"time\" id=\"toTime\" name=\"toTime\">\n";
  rHTMLString += "  </td>\n";
  rHTMLString += "</tr>\n";
  rHTMLString += "</table>\n";

  rHTMLString += "<script>\n";
  rHTMLString += "var dt = new Date()\n";
  rHTMLString += "document.getElementById(\"datetime\").innerHTML = ((\"0\"+dt.getDate()).slice(-2)) +\".\"+ ((\"0\"+(dt.getMonth()+1)).slice(-2)) +\".\"+ (dt.getFullYear()) +\" \"+ ((\"0\"+dt.getHours()).slice(-2)) +\":\"+ ((\"0\"+dt.getMinutes()).slice(-2))\n";
  rHTMLString += "</script>\n";

  rHTMLString += "<script>\n";
  rHTMLString += "function updateTextInput(val) {\n";
  rHTMLString += "document.getElementById('textInput').value=val; }\n";
  rHTMLString += "</script>\n";

  rHTMLString += "<script>\n";
  rHTMLString += "function scheduleClicked(elem) {\n";
  rHTMLString += "if (elem.checked == true) {\n";
  rHTMLString += "  document.getElementById('fromTime').disabled = false;\n";
  rHTMLString += "  document.getElementById('toTime').disabled = false;\n";
  rHTMLString += "} else {\n";
  rHTMLString += "  document.getElementById('fromTime').disabled = true;\n";
  rHTMLString += "  document.getElementById('toTime').disabled = true;\n";
  rHTMLString += "}\n";
  rHTMLString += "</script>\n";

  rHTMLString += "</body></html>\n";

  // The HTTP response ends with another blank line
  rHTMLString += "\n";
}

void LEDController::SetNumLEDs(int iNumLEDs)
{
	if (m_printFunc != nullptr)
		m_printFunc("LEDController::SetNumLEDs");

	m_spData->iNumLEDs = iNumLEDs;
	if (m_spData->pLeds != nullptr)
	{
		delete[] m_spData->pLeds;
	}
	m_spData->pLeds = new CRGB[m_spData->iNumLEDs];
}

void LEDController::SetBrightness(uint8_t iBrightness)
{
	m_spData->iBrightness = iBrightness;
}

void LEDController::SetUpdateRate(int iUpdateRate)
{
	m_spData->iUpdateRate = iUpdateRate;
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

void LEDController::Setup(uint8_t iLEDPin, int iNumLEDs, uint8_t iBrightness, int iUpdateRate)
{
	if (m_printFunc != nullptr)
		m_printFunc("LEDController::Setup");

	m_spData = make_unique<SData>();
	
	m_spData->iLEDPin = iLEDPin;
	m_spData->iNumLEDs = iNumLEDs;
	m_spData->iBrightness = iBrightness;
	m_spData->iUpdateRate = iUpdateRate;
	
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
	}
	
	// LEDs
	if (m_spData->iLEDPin == 27)
	{
		if (m_printFunc != nullptr)
			m_printFunc("LEDController::addLeds");
		FastLED.addLeds<CHIPSET, LED_PIN_27, COLOR_ORDER>(m_spData->pLeds, m_spData->iNumLEDs).setCorrection( TypicalLEDStrip );
	}
	FastLED.setBrightness( m_spData->iBrightness );

	// Set all black.
	LEDBase::SetBlack(m_spData->pLeds, m_spData->iNumLEDs);
}

void LEDController::Loop()
{
	for (int i = 0; i < m_spData->availableLEDFunctions.size(); ++i)
	{
		if (m_spData->availableLEDFunctions[i]->IsActive())
		{
			m_spData->availableLEDFunctions[i]->UpdateLEDs(m_spData->pLeds, m_spData->iNumLEDs);
		}
	}

	FastLED.show(); // display this frame
	FastLED.delay(1000 / m_spData->iUpdateRate);	
}

void LEDController::OnEvent(const String& rURL, std::vector<std::pair<String, String>>& rArguments, String& rHTMLString)
{
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
