#ifndef LEDFIRE_H
#define LEDFIRE_H

#pragma once

#include "LEDBase.h"

class LEDFire : public LEDBase
{
	public:
		LEDFire(void);
		virtual ~LEDFire(void);
		
	public:
		virtual String GetSubPage(void) const override;
		virtual String GetSubPageLink(void) const override;
		virtual void CreateHTML(String& rHTMLString) override;
		virtual void UpdateLEDs(CRGB* leds, int iNumLEDs) override;
		
	protected:
		virtual void onEvent(std::vector<std::pair<String, String>>& rArguments) override;
	
	private:
		uint8_t* heat = nullptr;
};
#endif