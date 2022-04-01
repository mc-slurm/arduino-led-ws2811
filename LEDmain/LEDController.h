#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <functional>
#include "WString.h"
#include "Singleton.h"

/**
	Only for chipset WS2811 and color order GRB
*/
class LEDController : public Singleton<LEDController>
{
	public:
		LEDController();
		virtual ~LEDController(void);

	public:
		void CreateHTML(String& rHTMLString);
		void SetNumLEDs(int iNumLEDs);
		void SetBrightness(uint8_t iBrightness);
		void SetUpdateRate(int iUpdateRate);
		void SetURL(const String& rURL);
		void SetTime(uint8_t uiHour, uint8_t uiMinute);
		
	public:
		void Setup(const String& rURL, uint8_t iLEDPin, int iNumLEDs, uint8_t iBrightness, int iUpdateRate);
		void Loop();
		void OnEvent(const String& rURL, std::vector<std::pair<String, String>>& rArguments, String& rHTMLString);
		
		void ShowErrorLED(void);
		
		std::vector<String> GetSubPageLinks(void) const;
		
	public:
		void SaveConfigs(void) const;
		void LoadConfigs(void);
		
	public:
		void RegisterPrintFunction(std::function<void(const String&)> rFunc) { m_printFunc = rFunc; }
		
	protected:
		void registerLEDs(void);
		
		struct SData;
		std::unique_ptr<SData> m_spData;
		std::function<void(const String&)> m_printFunc;
};
#endif