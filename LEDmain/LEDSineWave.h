#ifndef LEDSINEWAVE_H
#define LEDSINEWAVE_H

#pragma once

#include "LEDBase.h"

/** y(x,t) = A * sin(kx-wt+o)+D
	k = 2*pi / wavelength
	w = 2*pi * f, (f = angular frequency in radians per sec)
	o = phase angle (in radians)
*/
class LEDSineWave : public LEDBase
{
	public:
		LEDSineWave(void);
		virtual ~LEDSineWave(void);
		
	public:
		virtual String GetSubPage(void) const override;
		virtual String GetSubPageLink(void) const override;
		virtual void CreateHTML(String& rHTMLString) override;
		virtual void UpdateLEDs(CRGB* leds, int iNumLEDs) override;
		
	protected:
		virtual void onEvent(std::vector<std::pair<String, String>>& rArguments) override;
		std::unique_ptr<LEDConfigBase> createConfig(const String& rName);
	
	private:
		float m_fTime = 0.f;
};
#endif