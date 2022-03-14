#ifndef LEDSHOT_H
#define LEDSHOT_H

#pragma once

#include "LEDBase.h"

class LEDShot : public LEDBase
{
	public:
		LEDShot(void);
		virtual ~LEDShot(void);
		
	public:
		virtual String GetSubPage(void) const override;
		virtual String GetSubPageLink(void) const override;
		virtual void CreateHTML(String& rHTMLString) override;
		virtual void UpdateLEDs(CRGB* leds, int iNumLEDs) override;
		
	protected:
		virtual void onEvent(std::vector<std::pair<String, String>>& rArguments) override;
		std::unique_ptr<LEDConfigBase> createConfig(const String& rName);
	
	private:
		uint8_t m_uiCurTime = 0;
		uint8_t m_uiTime = 100;
		uint8_t m_uiShotPosition = UINT8_MAX;
};
#endif