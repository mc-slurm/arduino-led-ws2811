#ifndef LEDBASE_H
#define LEDBASE_H

#pragma once

#include <Arduino.h>
#include <functional>
#include <memory>
#include <vector>
#include "LEDConfigBase.h"
class CRGB;
class StreamEEPROM;

class LEDBase
{
	public:
		LEDBase(void) {}
		virtual ~LEDBase(void) {}

	public:
		void SetActive(bool bActive) { m_bActive = bActive; }
		bool IsActive(void) const { return m_bActive; }
		void OnEvent(std::vector<std::pair<String, String>>& rArguments) { onEvent(rArguments); }
		
		static void SetBlack(CRGB* leds, int iNumLEDs);
		
	public:
		virtual String GetSubPage(void) const = 0;
		virtual String GetSubPageLink(void) const = 0;
		virtual void CreateHTML(String& rHTMLString) = 0;
		virtual void UpdateLEDs(CRGB* leds, int iNumLEDs) = 0;
	
	public:
		void SetUpdateFrequency(const int& iFrequency) { m_iUpdateFrequency = iFrequency; }
		void SetURL(const String& rURL) { m_URL = rURL; }

	public:
		static void CreateHTMLHeader(const String& rTitle, String& rHTMLString);
		static void CreateHTMLOnOffButtons(const String& rSubPageLink, bool bActive, String& rHTMLString);
		static void CreateHTMLFooter(String& rHTMLString);

		static void CreateHTMLUpdateValueFunction(const String& rURL, const String& rSubPage, String& rHTMLString);
		static void CreateHTMLAddConfigFunction(const String& rURL, const String& rSubPage, String& rHTMLString);
		static void CreateHTMLRGBToHexFunction(String& rHTMLString);
		static void CreateHTMLDateTimeFunction(String& rHTMLString);
		
		static void CreateHTMLConfigTableRow(std::vector<std::unique_ptr<LEDConfigBase>>& rConfigs, const String& rSubPageLink, const String& rActiveConfigName, String& rHTMLString);

	public:
		void Serialize(StreamEEPROMWrite& rStream) const;
		void Serialize(const StreamEEPROMRead& rStream);	

	protected:
		virtual void onEvent(std::vector<std::pair<String, String>>& rArguments);
		virtual std::unique_ptr<LEDConfigBase> createConfig(const String& rName) = 0;
		void assertAndSetCustomConfig(void);
		bool isConfigNameReserved(const String& rName) const;
		bool isConfigNameExists(const String& rName) const;
		bool m_bActive = false;
		int m_iUpdateFrequency = 100;
		String m_URL = "";
		
	protected:
		template<typename T>
		T& getConfig(uint32_t uiIndex) { return static_cast<T&>(*m_configurations[uiIndex]); }

		template<typename T>
		T& getActiveConfig(void) { return static_cast<T&>(*m_configurations[m_uiActiveConfig]); }
		
		std::vector<std::unique_ptr<LEDConfigBase>> m_configurations;
		uint32_t m_uiActiveConfig = 0;
		
		static String getConfigActionString(void);
		static String getConfigCreateString(void);
		static String getConfigDeleteString(void);

	
	private:
};
#endif