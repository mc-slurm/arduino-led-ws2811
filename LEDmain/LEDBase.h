#ifndef LEDBASE_H
#define LEDBASE_H

#pragma once

#include <Arduino.h>
#include <functional>
class CRGB;

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
		void RegisterPrintFunction(std::function<void(const String&)> rFunc) { m_printFunc = rFunc; }

	public:
		static void CreateHTMLHeader(const String& rTitle, String& rHTMLString);
		static void CreateHTMLOnOffButtons(const String& rSubPageLink, bool bActive, String& rHTMLString);
		static void CreateHTMLFooter(String& rHTMLString);

		static void CreateHTMLUpdateValueFunction(const String& rURL, const String& rSubPage, String& rHTMLString);
		static void CreateHTMLRGBToHexFunction(String& rHTMLString);
		static void CreateHTMLDateTimeFunction(String& rHTMLString);

	protected:
		virtual void onEvent(std::vector<std::pair<String, String>>& rArguments) {}
		bool m_bActive = false;
		int m_iUpdateFrequency = 100;
		String m_URL = "";
		
		std::function<void(const String&)> m_printFunc;
	
	private:
};
#endif