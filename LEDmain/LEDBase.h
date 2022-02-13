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
		void RegisterPrintFunction(std::function<void(const String&)> rFunc) { m_printFunc = rFunc; }

	protected:
		virtual void onEvent(std::vector<std::pair<String, String>>& rArguments) {}
		bool m_bActive = false;
		int m_iUpdateFrequency = 100;
		std::function<void(const String&)> m_printFunc;
	
	private:
};
#endif