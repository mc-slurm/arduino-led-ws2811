#ifndef LEDRGB_H
#define LEDRGB_H

#pragma once

#include "LEDBase.h"

class LEDRGB : public LEDBase
{
	public:
		LEDRGB(void);
		virtual ~LEDRGB(void);
		
	public:
		virtual String GetSubPage(void) const override;
		virtual String GetSubPageLink(void) const override;
		virtual void CreateHTML(String& rHTMLString) override;
		virtual void UpdateLEDs(CRGB* leds, int iNumLEDs) override;
		
	protected:
		virtual void onEvent(std::vector<std::pair<String, String>>& rArguments) override;
	
	private:
		uint8_t m_iRed = 128;
		uint8_t m_iBlue = 128;
		uint8_t m_iGreen = 128;
};
#endif